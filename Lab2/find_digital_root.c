#include "types.h"
#include "user.h"

int fdr_syscall(int num) {
    int prev_ebx;

    // Saving ebx in prev_ebx (to restore later)
    // Move num to ebx
    asm volatile(
        "movl %%ebx, %0\n\t"
        "movl %1, %%ebx"
        : "=r"(prev_ebx)
        : "r"(num)
    );

    int result = find_digital_root();

    // Restoring ebx.
    asm volatile(
        "movl %0, %%ebx"
        :: "r"(prev_ebx)
    );

    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf(2, "Error: find_gital_root <number>\n");
        exit();
    }

    if(argv[1][0] == '-') {
        printf(2, "Number not accepting\n");
    } else {
        int num = atoi(argv[1]);

        int result = fdr_syscall(num);
        if (result == -1) {
            printf(2, "Error: Number not accepting\n");
        } else {
            printf(1, "%d\n", result);
        }

    }
    exit();
}
