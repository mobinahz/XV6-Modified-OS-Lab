# Operating System Lab projects Fall-2023
- Operating System Lab Projects F2023
  - [Intro](##Intro)
  - Experiment 1: Introduction to xv6
    - Task 1: Boot Message
    - Task 2: Console Features
    - Task 3: User Program
  - Experiment 2: System Calls
    - Task 1: System Call Using Registers
    - Task 2: System Call Using Stack
  - Experiment 3: Process Scheduling
    - Task 1: Multi-Level Feedback Queue
    - Task 2: Aging
    - Task 3: System Calls
  - [Experiment 4: Synchronization](##Experiment 4: Synchronization)
    - Task 1: Number of System Calls
    - Task 2: Priority Lock
  - Experiment 5: Memory Management
 
## Intro
In these projects, additional features were implemented in the xv6 operating system, which is designed for the x86 architecture.

## Experiment 1: Introduction to xv6
### Task 1: Boot Message
The system displays the names of the group members as a message when it boots up.
### Task 2: Console Features
The following shortcuts are added to the console:

- `Ctrl+N:` Deletes all digits from the console.
- `Ctrl+R:` Reverses the current line.
- `Tab:` Replaces the current line with a command from the history, if available.
- `Ctrl+B:` Moves the cursor one character to the left.
- `Ctrl+F:` Moves the cursor one character to the right.
- `Ctrl+L:` Clears the console.
- `Arrow Up:` Displays the previous command from the history.
- `Arrow Down:` Displays the next command from the history.
### Task 3: User Program  
A strdiff program has been introduced to the system. This program compares two strings to identify their differences. It is used as follows:
```Python
strdiff a b
```
The output is saved in a file named `strdiff_result.txt`.

## Experiment 2: System Calls
### Task 1: System Call Using Registers
A new system call, `find_digit_root`, has been added to the system. This system call calculates the largest prime factor of a given number. It can be invoked as follows:
```CPP
int find_digit_root(void);
```
The integer parameter should be passed via the ebx register.

### Task 2: System Call Using Stack

The following system calls have been added to the system:
- `copy_file`: Copies a file to a specified location.
```CPP
int copy_file(char *path, int size);
```
- `get_uncle_count`: Returns the number of uncle processes for the current process.
```CPP
void get_uncle_count(void);
```
- `get_process_lifetime`: Retrieves the lifetime of the current process.
```CPP
int get_process_lifetime(void);
```
## Experiment 3: Process Scheduling
### Task 1: Multi-Level Feedback Queue
A Multi-Level Feedback Queue (MLFQ) scheduler has been implemented in the system. It consists of three queues, with the highest priority assigned to the first queue. The scheduling policies for these queues are as follows:

- The first queue operates on a round-robin basis with a time quantum of 1 tick.
- The second queue uses a Last-Come-First-Served (LCFS) policy, where the most recently arrived process is executed first.
- The third queue employs a Best-Job-First (BJF) policy, executing processes with the lowest rank first.

### Task 2: Aging
All processes are initially placed in the second queue, with the exception of the init and sh processes. If a process in the second queue remains runnable but is not executed for 8000 ticks, it is promoted to the first queue.

### Task 3: System Calls
The following system calls have been introduced:
- `change_scheduling_queue`: Modifies the scheduling queue of a specific process.
```CPP
int change_scheduling_queue(int pid, int queue);
```
- `set_bjf_params_process`: Configures the BJF algorithm parameters for a particular process.
```CPP
int set_bjf_params_process(int pid, float priority_ratio, float arrival_time_ratio, float executed_cycles_ratio);
```
- `set_bjf_params_system`: Configures the BJF algorithm parameters for the entire system.
```CPP
int set_bjf_params_system(float priority_ratio, float arrival_time_ratio, float executed_cycles_ratio);
```
- `print_process_info`: Displays process information in a tabular format.
```CPP
void print_process_info(void);
```

## Experiment 4: Synchronization
### Task 1: Number of System Calls
As part of the synchronization experiment, we count the number of system calls. First, we ensure the operating system is running on 4 cores, then we execute the getnsyscalls program. The program is called using:
```CPP
int getnsyscalls();
```
The result, which includes the count of system calls on each core and their total, is displayed on the console. To ensure accurate counting, we use spin locks to prevent any process from being interrupted during the counting process.
### Task 2: Priority Lock
A priority lock has been introduced into the system, operating based on the spin lock policy. The following system calls have been added to manage this lock:
```CPP
void acquirepriority(struct prioritylock* lk);
void releasepriority(struct prioritylock* lk);
```
## Experiment 5: Memory Management








