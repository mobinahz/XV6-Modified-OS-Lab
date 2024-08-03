#include "user.h"
#include "types.h"

int main(int argc, char* argv[])
{
    int check;
    if (argc != 3)
    {
        printf(1, "not enough arguemts\n");
        exit();
    }
    if(argv[1] == argv[2])
    {
        printf(1, "same name, cannot allow\n");
        exit();
    }
    if((check = copy_file(argv[1], argv[2]) < 0))
    {
        printf(1,"error\n");
        exit();
    }
    exit();
}