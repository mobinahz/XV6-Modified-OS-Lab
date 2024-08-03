# Operating System Lab projects Fall-2023
- Operating System Lab Projects F2023
  - Intro
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
  - Experiment 4: Synchronization
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

## 
