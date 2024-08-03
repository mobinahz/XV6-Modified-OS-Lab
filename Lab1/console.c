// Console input and output.
// Input is from the keyboard or serial port.
// Output is written to the screen and serial port.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

static void consputc(int);

static int panicked = 0;

static struct
{
  struct spinlock lock;
  int locking;
} cons;

static void
printint(int xx, int base, int sign)
{
  static char digits[] = "0123456789abcdef";
  char buf[16];
  int i;
  uint x;

  if (sign && (sign = xx < 0))
    x = -xx;
  else
    x = xx;

  i = 0;
  do
  {
    buf[i++] = digits[x % base];
  } while ((x /= base) != 0);

  if (sign)
    buf[i++] = '-';

  while (--i >= 0)
  {
    consputc(buf[i]);
  }
}
// PAGEBREAK: 50

// Print to the console. only understands %d, %x, %p, %s.
void cprintf(char *fmt, ...)
{
  int i, c, locking;
  uint *argp;
  char *s;

  locking = cons.locking;
  if (locking)
    acquire(&cons.lock);

  if (fmt == 0)
    panic("null fmt");

  argp = (uint *)(void *)(&fmt + 1);
  for (i = 0; (c = fmt[i] & 0xff) != 0; i++)
  {
    if (c != '%')
    {
      consputc(c);
      continue;
    }
    c = fmt[++i] & 0xff;
    if (c == 0)
      break;
    switch (c)
    {
    case 'd':
      printint(*argp++, 10, 1);
      break;
    case 'x':
    case 'p':
      printint(*argp++, 16, 0);
      break;
    case 's':
      if ((s = (char *)*argp++) == 0)
        s = "(null)";
      for (; *s; s++)
        consputc(*s);
      break;
    case '%':
      consputc('%');
      break;
    default:
      // Print unknown % sequence to draw attention.
      consputc('%');
      consputc(c);
      break;
    }
  }

  if (locking)
    release(&cons.lock);
}

void panic(char *s)
{
  int i;
  uint pcs[10];

  cli();
  cons.locking = 0;
  // use lapiccpunum so that we can call panic from mycpu()
  cprintf("lapicid %d: panic: ", lapicid());
  cprintf(s);
  cprintf("\n");
  getcallerpcs(&s, pcs);
  for (i = 0; i < 10; i++)
    cprintf(" %p", pcs[i]);
  panicked = 1; // freeze other CPU
  for (;;)
    ;
}

// PAGEBREAK: 50
#define BACKSPACE 0x100
#define LEFTARROW 75
#define RIGHTARROW 76
#define ARROWUP 0xE2
#define ARROWDOWN 0xE3
#define CRTPORT 0x3d4
static ushort *crt = (ushort *)P2V(0xb8000); // CGA memory

#define QUEUESIZE 10
#define INPUT_BUF 128
struct
{
  char buf[QUEUESIZE][INPUT_BUF];

  uint num;           // tedad command hayi ke tahala vared shode
  uint arrow_counter; // chanbar arrowup zadam va kodomo run konam
  uint last_element_of_queue;
  uint first_element_of_queue;
  uint last_read;
  // harbar command e vorudi ro save konam to in array -> consintr
  // iterate in this array -> arrow up
  // command ro toye input.buf benevisam va bahash mesle yek command jadid raftar konam ->arrow up

} commands;

#define INPUT_BUF 128
struct
{
  char buf[INPUT_BUF];
  uint r; // Read index
  uint w; // Write index
  uint e; // Edit index
  uint p; // pointer for code edit
} input;

uint find_index()
{
  uint temp_last_element = commands.last_element_of_queue;
  while (temp_last_element >= 0 && commands.arrow_counter > 0)
  {
    temp_last_element--;
    commands.arrow_counter--;
  }
  for (uint i = QUEUESIZE - 1; i > QUEUESIZE - commands.last_element_of_queue; i--)
  {
    commands.arrow_counter--;
    if (commands.arrow_counter == 0)
    {
      return i;
    }
  }
  return 9;
}

void clean_line()
{
  while (input.e != input.w &&
         input.buf[(input.e - 1) % INPUT_BUF] != '\n')
  {
    input.e--;
    consputc(BACKSPACE); // yek character mizare ro console
  }
}

void write_str_in_console(const char *s)
{
  clean_line();
  input.p = input.e = input.w;
  for (int i = 0; i < INPUT_BUF; ++i)
  {
    if (s[i] == '\0')
    {
      return;
    }
    input.buf[input.p++ % INPUT_BUF] = s[i];
    input.e++;
    consputc(s[i]);
  }
}

void arrow_up()
{
  if (commands.num == 0)
  {
    clean_line();
    return;
  }

  uint index;

  index = commands.last_read;

  write_str_in_console(commands.buf[index]);
  commands.last_read++;
}

void arrow_down()
{
  if (commands.num == 0)
  {
    clean_line();
    return;
  }

  uint index;

  commands.last_read--;
  index = commands.last_read;

  write_str_in_console(commands.buf[index]);
}

void delete_buf()
{
  while (input.e != input.w &&
         input.buf[(input.e - 1) % INPUT_BUF] != '\n')
  {
    input.e--;
    input.buf[input.e] = '\0';
  }
}

static void
cgaputc(int c)
{
  int pos;
  int bs_flag = 0;
  // Cursor position: col + 80*row.
  outb(CRTPORT, 14);
  pos = inb(CRTPORT + 1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT + 1);
  if (c == '\n')
  {
    if (input.e < input.p)
      pos += pos + input.e - input.p;
    pos += 80 - pos % 80;
  }
  else if (c == BACKSPACE)
  {
    if (pos > 0)
    {
      --pos;
      bs_flag = 1;
      if (input.p < input.e)
      {
        int temp_pos = pos;
        int temp_pos2 = input.p;
        bs_flag = 0;
        for (uint i = input.p; i < input.e; i++)
        {
          crt[temp_pos] = crt[temp_pos + 1];
          input.buf[temp_pos2] = input.buf[temp_pos2 + 1];
          temp_pos++;
          temp_pos2++;
        }
        input.e--;
      }
    }
  }
  else if (c == LEFTARROW)
  {
    if (pos > 0)
      --pos;
  }
  else if (c == RIGHTARROW)
  {
    if (input.p <= input.e + 1)
    {
      pos++;
    }
  }
  else
  {
    if (input.p < input.e)
    {
      int tmp_pos = pos + input.e - input.p;
      for (uint i = input.e; i > input.p; i--)
      {
        crt[tmp_pos] = crt[tmp_pos - 1];
        tmp_pos--;
      }
    }
    crt[pos++] = (c & 0xff) | 0x0700; // black on white
  }

  if (pos < 0 || pos > 25 * 80)
    panic("pos under/overflow");

  if ((pos / 80) >= 24)
  { // Scroll up.
    memmove(crt, crt + 80, sizeof(crt[0]) * 23 * 80);
    pos -= 80;
    memset(crt + pos, 0, sizeof(crt[0]) * (24 * 80 - pos));
  }

  outb(CRTPORT, 14);
  outb(CRTPORT + 1, pos >> 8);
  outb(CRTPORT, 15);
  outb(CRTPORT + 1, pos);
  if (bs_flag)
    crt[pos] = ' ' | 0x0700;
  bs_flag = 0;
}

void write_to_commands(uint num)
{
  uint i = 0;
  uint temp_w = input.w;
  while (input.e != temp_w)
  {
    commands.buf[num][i] = input.buf[temp_w];
    temp_w++;
    i++;
  }
}

int get_pos()
{
  int pos;
  outb(CRTPORT, 14);
  pos = inb(CRTPORT + 1) << 8;
  outb(CRTPORT, 15);
  pos |= inb(CRTPORT + 1);

  return pos;
}

void consputc(int c)
{
  if (panicked)
  {
    cli();
    for (;;)
      ;
  }

  if (c == BACKSPACE)
  {
    uartputc('\b');
    uartputc(' ');
    uartputc('\b');
  }
  else if (c == LEFTARROW)
  {
    //
  }
  else if (c == RIGHTARROW)
  {
    //
  }
  else
    uartputc(c);
  cgaputc(c);
}

void delete_each_element(char *s)
{
  for (uint i = 0; i < INPUT_BUF; i++)
  {
    s[i] = '\0';
  }
}

int find_size(char *s)
{
  for (int i = 0; i < INPUT_BUF; i++)
  {
    if (s[i] == '\0')
      return i;
  }
  return INPUT_BUF;
}

void save_command()
{
  if (input.e - input.w == 1)
  {
    return;
  }

  for (uint i = QUEUESIZE - 1; i > 0; i--)
  {
    for (uint j = 0; j < INPUT_BUF; j++)
    {
      if (commands.buf[i - 1][j] == '\0')
      {
        commands.buf[i][j] = '\0';
        break;
      }
      commands.buf[i][j] = commands.buf[i - 1][j];
    }
  }

  int j = 0;
  for (int i = input.w; i < input.e + 1; i++)
  {
    if (input.buf[i] == '\n')
    {
      commands.buf[0][j] = '\0';
      break;
    }
    commands.buf[0][j] = input.buf[i];
    j++;
  }
  commands.last_element_of_queue++;
  commands.last_read = 0;
  commands.num++;
}

void write_in_specific_position(int c, int pos)
{
  outb(CRTPORT, 14);
  outb(CRTPORT + 1, pos >> 8);
  outb(CRTPORT, 15);
  outb(CRTPORT + 1, pos);
  crt[pos] = c | 0x0700;
}

void relocate_cursor(int pos)
{
  outb(CRTPORT, 14);
  outb(CRTPORT + 1, pos >> 8);
  outb(CRTPORT, 15);
  outb(CRTPORT + 1, pos);
  crt[pos] = ' ' | 0x0700;
}

#define C(x) ((x) - '@') // Control-x
void consoleintr(int (*getc)(void))
{
  int c, doprocdump = 0;
  acquire(&cons.lock);
  while ((c = getc()) >= 0)
  {
    switch (c)
    {
    case C('P'): // Process listing.
      // procdump() locks cons.lock indirectly; invoke later
      doprocdump = 1;
      break;
    case C('U'): // Kill line.
      while (input.e != input.w &&
             input.buf[(input.e - 1) % INPUT_BUF] != '\n')
      {
        if (input.p != input.e)
          input.p = input.e;
        input.e--;
        input.p--;
        consputc(BACKSPACE);
      }
      break;
    case C('H'):
    case '\x7f': // Backspace
      if (input.e != input.w && input.p != input.w)
      {
        if (input.e == input.p)
        {
          input.e--;
          input.p--;
        }
        else
        {
          input.p--;
        }
        consputc(BACKSPACE);
      }
      break;
    case C('B'): // LeftArrow
      if (input.p > input.w)
      {
        input.p--;
        consputc(LEFTARROW);
      }
      break;
    case C('F'): // RightArrow
      if (input.p < input.e)
      {
        input.p++;
        consputc(RIGHTARROW);
      }
      break;
    case C('L'):
      clean_line();
      int position = get_pos();

      while (position >= 0)
      {
        crt[position--] = ' ' | 0x0700;
      }
      write_in_specific_position('$', 0);
      relocate_cursor(2);
      break;
    case ARROWUP:
      clean_line();
      arrow_up();
      break;
    case ARROWDOWN:
      clean_line();
      arrow_down();
      break;
    default:
      if (c != 0 && input.e - input.r < INPUT_BUF)
      {
        c = (c == '\r') ? '\n' : c;
        if (input.p < input.e && c != '\n')
        {
          for (uint i = input.e; i > input.p; i--)
          {
            input.buf[(i) % INPUT_BUF] = input.buf[(i - 1) % INPUT_BUF];
          }
        }
        // input.e++;
        if (c != '\n')
        {
          input.buf[input.p++ % INPUT_BUF] = c;
        }
        consputc(c);
        if (c == '\n' || c == C('D') || input.e == input.r + INPUT_BUF)
        {
          save_command();
          input.buf[input.e++ % INPUT_BUF] = c;
          input.p = input.e;
          input.w = input.e;
          // input.buf[input.e] = c;
          wakeup(&input.r);
        }
        else
        {
          input.e++;
        }
      }
      break;
    }
  }
  release(&cons.lock);
  if (doprocdump)
  {
    procdump(); // now call procdump() wo. cons.lock held
  }
}

int consoleread(struct inode *ip, char *dst, int n)
{
  uint target;
  int c;

  iunlock(ip);
  target = n;
  acquire(&cons.lock);
  while (n > 0)
  {
    while (input.r == input.w)
    {
      if (myproc()->killed)
      {
        release(&cons.lock);
        ilock(ip);
        return -1;
      }
      sleep(&input.r, &cons.lock);
    }
    c = input.buf[input.r++ % INPUT_BUF];
    if (c == C('D'))
    { // EOF
      if (n < target)
      {
        // Save ^D for next time, to make sure
        // caller gets a 0-byte result.
        input.r--;
      }
      break;
    }
    *dst++ = c;
    --n;
    if (c == '\n')
      break;
  }
  release(&cons.lock);
  ilock(ip);

  return target - n;
}

int consolewrite(struct inode *ip, char *buf, int n)
{
  int i;

  iunlock(ip);
  acquire(&cons.lock);
  for (i = 0; i < n; i++)
    consputc(buf[i] & 0xff);
  release(&cons.lock);
  ilock(ip);

  return n;
}

void consoleinit(void)
{
  initlock(&cons.lock, "console");

  devsw[CONSOLE].write = consolewrite;
  devsw[CONSOLE].read = consoleread;
  cons.locking = 1;

  ioapicenable(IRQ_KBD, 0);
}
