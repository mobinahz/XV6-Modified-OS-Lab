// Priority locks

#include "prioritylock.h"

void
init_prioritylock(struct prioritylock *lk, char *name)
{
  initlock(&lk->lk, NAME);
  lk->name = name;
  lk->locked = FALSE;
  lk->pid = ZERO;

  for (int i = ZERO; i < NPROC; ++i)
  {
    lk->queue[i] = ZERO;
  }
}

void
acquire_priority(struct prioritylock *lk)
{
  acquire(&lk->lk);
  boolean first = queue_is_empty(lk);

  if (first && !lk->locked)
  {
    lk->locked = TRUE;
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
    lk->locked = FALSE;
    lk->pid = ZERO;
  }
  else 
  {
    lk->locked = TRUE;
    lk->pid = dequeue(lk);
  }
  wakeup(lk);

  release(&lk->lk);
}

int 
dequeue(struct prioritylock *lk)
{
  int target = lk->queue[ZERO];

  for (int i = ZERO; i < NPROC-ONE; ++i)
  {
    if (lk->queue[i] == ZERO)
      break;

    lk->queue[i] = lk->queue[i+ONE];
  }

  return target;
}

void 
enqueue(struct prioritylock *lk, int pid)
{
  if (queue_is_empty(lk)){
    lk->queue[ZERO] = pid;
    return;
  }
  int index = ZERO;
  for (int i = ZERO; i < NPROC; ++i)
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
  for (int i = NPROC-ONE; i >= index+ONE; --i)
  {
    lk->queue[i] = lk->queue[i-ONE];
  }

  lk->queue[index] = pid;
}

boolean 
queue_is_empty(struct prioritylock *lk)
{
  if (lk->queue[ZERO] == ZERO)
    return TRUE;
  return FALSE;
}

boolean 
is_in_queue(struct prioritylock *lk, int pid)
{
  for (int i = ZERO; i < NPROC; ++i)
  {
    if (pid == lk->queue[i])
      return TRUE;
  }

  return FALSE;
}

void 
show_acquiring_info(struct prioritylock *lk)
{
  cprintf("Priority of Process in Critical Section: %d\n", lk->pid);
  cprintf("Processes in Queue:\n");

  for (int i = ZERO; i < NPROC; ++i)
  {
    if (lk->queue[i] == ZERO)
      break;
    else
      cprintf("%d. %d\n", i+ONE, lk->queue[i]);
  }
}