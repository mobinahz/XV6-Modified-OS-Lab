#include "types.h"
#include "user.h"
#include "fcntl.h"

void 
reverse(char* str, int len) 
{ 
    int i = 0, j = len - 1, temp; 
    while (i < j) { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; 
        j--; 
    }
} 


int convert_int_to_string(int index, char str[], int d) {

    int i = 0;

    if(index == 0)  str[i++] = '0';

    while(index) {
        str[i++] = (index % 10) + '0';
        index = index / 10;
    }

    while(i < d) {
        str[i++] = '0';
    }

    reverse(str, i);

    str[i] = '\0';
    return i;

}

void file_operations(int index)
{
    char filename[512];
    char str[512]= "multi-core test";
    
	convert_int_to_string(index, filename, 0);
    // printf(1, "filename: %s", filename);

    int fd = open(filename, O_CREATE | O_RDWR);
    if(fd < 0) printf(1, "file creation failed.\n");
    if(write(fd, str, sizeof(str)) != sizeof(str))
    {
        printf(1, "write to file failed.\n");
        exit();
    }
    // printf(1, "file operation\n");
    close(fd);
    exit();
}

int main(int argc, char *argv[])
{
    int pid = getpid();
    reset_syscall_counter();
    // printf(1, "initial pid: %d\n", pid);ridz
    for(int i = 0; i < 10; i++)
    {
        pid = fork();
        // printf(1, "pid: %d\n", pid);
        // if(pid > 0)
        //     wait();
        // else if(pid == 0) file_operations(pid);
        // else 
        // {
        //     printf(1, "forking failed.\n");
        //     exit();
        // }

    }
    printf(1, "forking done.\n");
    if(pid < 0)
    {
        printf(1, "forking failed.\n");
        exit();
    }
    else if(!pid)
    {
        file_operations(pid);
    }
    else
    {
    for(int i = 0; i < 10; i++)
        wait();
    get_syscall_counter();
    }
    // printf(1, "bade forking\n");
    exit();
}