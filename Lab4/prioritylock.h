#ifndef _PRIORITYLOCK_H_
#define _PRIORITYLOCK_H_
// Priority locks

#include "types.h"
#include "defs.h"
#include "param.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

#define LOCKNAME "priority lock"

typedef int boolean;

struct prioritylock {

  char *name;       
  int pid;      

  uint locked;       
  struct spinlock lk; 
  int queue[NPROC];
  
 
} prioritylock;

void init_prioritylock(struct prioritylock *lk, char *name);

int dequeue(struct prioritylock *lk);
void enqueue(struct prioritylock *lk, int pid);
void insert(struct prioritylock *lk, int pid, int index);

void acquire_priority(struct prioritylock *lk);
void release_priority(struct prioritylock *lk);

int queue_is_empty(struct prioritylock *lk);
int is_in_queue(struct prioritylock *lk, int pid);
void show_acquiring_info(struct prioritylock *lk);

#endif