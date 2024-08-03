#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "prioritylock.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_initpriority(void)
{
  char *name = "test";
  init_prioritylock(&prioritylock, name);

  return 0;
}

int
sys_testpriority(void)
{
  acquire_priority(&prioritylock);
  int z = 1;
  for(int j = 0; j < 20000; j+=1){
    z += (j + 1);
    cprintf ("");  
  }
  show_acquiring_info(&prioritylock);
  release_priority(&prioritylock);

  return 0;
}

int
sys_acquire_priority(void) {
  acquire_priority(&prioritylock);
  return 0;
}

int
sys_release_priority(void) {
  release_priority(&prioritylock);
  return 0;
}

int 
sys_resetsyscallnum(void)
{
  for (int i = 0; i < NCPU; ++i)
  {
    pushcli();
    cpus[i].syscallcounter = 0;
    __sync_synchronize();
    popcli();
  }

  pushcli();
  syscallcounter = 0;
  __sync_synchronize();
  popcli();

  return 0;
}

int
sys_getsyscallnum(void)
{
  int total = 0;
  int count = 0;
  cprintf("Shared system call counter value: %d\n", syscallcounter);

  for (int i = 0; i < NCPU; ++i)
  {
    pushcli();
    count = cpus[i].syscallcounter;
    popcli();

    total += count;
    cprintf("CPU %d system call counter value: %d\n", i, count);
  }

  cprintf("Sum of Per-cpu system call counters value: %d\n", total);

  return 0;
}


void
sys_init_sharedmem(void) {
  init_sharedmem();
}

void*
sys_open_sharedmem(void) {

  int id;
  argint(0, &id);
  return open_sharedmem(id);
}

void
sys_close_sharedmem(void) {

  int id;
  argint(0, &id);
  close_sharedmem(id);
}