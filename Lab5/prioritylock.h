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

#define ONE 1 
#define ZERO 0 
#define TRUE 1
#define FALSE 0
#define NAME "priority lock"

typedef int boolean;

struct prioritylock {
  uint locked;       // Is the lock held?
  struct spinlock lk; // spinlock protecting this priority lock
  int queue[NPROC];
  
  // For debugging:
  char *name;        // Name of lock.
  int pid;           // Process holding lock
} prioritylock;

void init_prioritylock(struct prioritylock *lk, char *name);
void acquire_priority(struct prioritylock *lk);
void release_priority(struct prioritylock *lk);
int dequeue(struct prioritylock *lk);
void enqueue(struct prioritylock *lk, int pid);
void insert(struct prioritylock *lk, int pid, int index);
boolean queue_is_empty(struct prioritylock *lk);
boolean is_in_queue(struct prioritylock *lk, int pid);
void show_acquiring_info(struct prioritylock *lk);

#endif