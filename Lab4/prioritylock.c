#include "prioritylock.h"

void
init_prioritylock(struct prioritylock *lk, char *name)
{
  initlock(&lk->lk, LOCKNAME);
  lk->name = name;
  lk->locked = 0;
  lk->pid = 0;

  for (int i = 0; i < NPROC; ++i)
  {
    lk->queue[i] = 0;
  }
}

void
acquire_priority(struct prioritylock *lk)
{
  acquire(&lk->lk);
  cprintf("Process %d acquired the lock\n", myproc()->pid);

  int head = queue_is_empty(lk);

  if (head && !lk->locked)
  {
    lk->locked = 1;
    lk->pid = myproc()->pid;
  }

  else 
  {
    if (!is_in_queue(lk, myproc()->pid))
      enqueue(lk, myproc()->pid);
    while (lk->locked && lk->pid != myproc()->pid) 
    {
      sleep(lk, &lk->lk);
    }
  }
  release(&lk->lk);
}

void
release_priority(struct prioritylock *lk)
{
  acquire(&lk->lk);

  if (queue_is_empty(lk)) 
  {
    lk->locked = 0;
    lk->pid = 0;
  }
  else 
  {
    lk->locked = 1;
    lk->pid = dequeue(lk);
  }
  wakeup(lk);

  release(&lk->lk);

  cprintf("Process %d is releasd\n", myproc()->pid);
}

int 
dequeue(struct prioritylock *lk)
{
  int target = lk->queue[0];

  for (int i = 0; i < NPROC - 1; ++i)
  {
    if (lk->queue[i] == 0)
      break;

    lk->queue[i] = lk->queue[i + 1];
  }

  return target;
}

void 
enqueue(struct prioritylock *lk, int pid)
{
  if (queue_is_empty(lk)){
    lk->queue[0] = pid;
    return;
  }
  int index = 0;
  for (int i = 0; i < NPROC; i++)
  {
    if (lk->queue[i] > pid) 
    {
      continue;
    }

    index = i;
    break;
  }

  insert(lk, pid, index);
}

void 
insert(struct prioritylock *lk, int pid, int index)
{
  for (int i = NPROC - 1; i >= index + 1; i--)
  {
    lk->queue[i] = lk->queue[i - 1];
  }

  lk->queue[index] = pid;
}

int 
queue_is_empty(struct prioritylock *lk)
{
  if (lk->queue[0] == 0)
    return 1;
  return 0;
}

int 
is_in_queue(struct prioritylock *lk, int pid)
{
  for (int i = 0; i < NPROC; i++)
  {
    if (pid == lk->queue[i])
      return 1;
  }

  return 0;
}

void 
show_acquiring_info(struct prioritylock *lk)
{
  cprintf("Priority of the Process in Critical Section: %d\n", lk->pid);
  cprintf("Content of the queue:\n");

  for (int i = 0; i < NPROC; i++)
  {
    if (lk->queue[i] == 0)
      break;
    else
      cprintf("%d ", lk->queue[i]);
  }
  cprintf("\n");
}