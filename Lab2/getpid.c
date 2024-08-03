#include "user.h"
#include "types.h"

int main()
{
    int pid = getpid();
    printf(1, "current process id is: %d\n", pid);
    exit();
    // return 0;
}