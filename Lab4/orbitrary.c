#include <stdio.h>
#include <malloc.h>

int main()
{
    char* a = malloc(sizeof(char));
    a[0] = 'c';
    a[1] = '2';
    char temp = 's';
    int i = 0;
    // temp = a[1];
    // a[1] = 's';
    // a[2] = temp;
    size_t size = sizeof(a) / sizeof(a[0]); /// sizeof(char);
    a[i++] = 'k';
    printf("%c, %d\n", a[0], i);   
    printf("%ld\n", size);
    return 0;
}