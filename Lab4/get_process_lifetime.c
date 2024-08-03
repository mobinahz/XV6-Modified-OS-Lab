#include "types.h"
#include "user.h"

int main() {
    int lifetime;
    
    int fork_pid = fork();
    if (fork_pid < 0) {
        printf(2, "Fork error\n");
        exit();
    }
    else if(fork_pid == 0) {
        sleep(1000);
        lifetime  = get_process_lifetime();
        printf(1, "Child lifetime is %d\n", lifetime);
    }
    else {
        wait();
        sleep(200);
        lifetime = get_process_lifetime();
        printf(1, "Parent lifetime is %d\n", lifetime);
    }
    exit();
}