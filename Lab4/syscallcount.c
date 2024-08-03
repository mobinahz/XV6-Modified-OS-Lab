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


int 
integer_to_string(int x, char str[], int d) 
{ 
    int i = 0; 

    if(x == 0)
      str[i++] = '0';

    while (x) { 
        str[i++] = (x % 10) + '0'; 
        x = x / 10; 
    } 
  
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 

void
call_file_syscalls(int index)
{
	char filename[10];
	char message[25] = "Per-cpu tests";

	integer_to_string(index, filename, 0);

	int fd = open(filename, O_CREATE | O_RDWR);

	if(fd < 0) 
	{
        printf(1, "create file error!\n");
        exit();
    }

	if(write(fd, message, sizeof(message)) != sizeof(message))
	{
        printf(1, "write to file error!\n");
        exit();
    }
    close(fd);
}

int 
main(int argc, char const *argv[])
{
	int pid = getpid();
	int i;
	resetsyscallnum();

	for (i = 1; i < 10; ++i)
	{
		if (pid > 0)
		{
			pid = fork();
		}
	}

	if(pid < 0)
    {
        printf(2, "fork error!\n");
    }
	else if (pid == 0)
	{
		call_file_syscalls(getpid());
	}
	else
	{
		for (i = 0; i < 10; i++)
			wait();

		getsyscallnum();

		printf(1, "Done :)\n");
	}
	
	exit();
}
