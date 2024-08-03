#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define SIZE_BUFF 14

char buf[SIZE_BUFF];

char *tolower(const char *word)
{
    int length = 0;
    while (word[length] != '\0')
    {
        length++;
    }

    char *lowerWord = (char *)malloc((length + 1) * sizeof(char));
    int i;
    for (i = 0; i < length; i++)
    {
        if (word[i] >= 'A' && word[i] <= 'Z')
        {
            lowerWord[i] = word[i] + 32; // Convert uppercase to lowercase using ASCII values
        }
        else
        {
            lowerWord[i] = word[i];
        }
    }
    lowerWord[length] = '\0'; // Add null-terminator to the end of the string

    return lowerWord;
}
int length(char *word)
{
    int length = 0;
    while (word[length] != '\0')
    {
        length++;
    }
    return length;
}

void strdiff(char word[], char word2[])
{
    char *wordT = tolower(word);
    char *wordT2 = tolower(word2);
    int sizediffer;
    int counter;
    counter = 0;

    int w1 = 0;
    if (length(wordT) < SIZE_BUFF && length(wordT2) < SIZE_BUFF)
    {
        while (wordT[w1] != '\0' && wordT2[w1] != '\0')
        {
            if (wordT[w1] >= wordT2[w1])
            {
                buf[w1] = '0';
            }

            if (wordT[w1] < wordT2[w1])
            {
                buf[w1] = '1';
            }
            w1++;
            counter++;
        }
        sizediffer = length(wordT) - length(wordT2);
        if (sizediffer > 0)
        {
            for (int i = w1; i < w1 + sizediffer; i++)
            {
                buf[w1] = '0';
                counter++;
            }
        }
        else if (sizediffer < 0)
        {
            for (int i = w1; i < w1 - sizediffer; i++)
            {
                buf[w1] = '1';
                counter++;
            }
        }
    }
    else{
        printf(2, "Size of words is more than threshold");
    }
    // buf[counter + 1] = '\n';
}

int main(int argc, char *argv[])
{
    int fd;
    char empty[SIZE_BUFF];

    if (argc < 3)
    {
        printf(2, "there is nothing to compare ");
        exit();
    }

    if (argc == 3)
    {
        strdiff(argv[1], argv[2]);
        // exit();
    }

    if (argc > 3)
    {
        printf(2, "syntax error we cant have more than 2");
        exit();
    }
    fd = open("strdiff_result.txt", O_CREATE | O_WRONLY);
    if (fd < 0)
    {
        printf(2, "Failed to open the file.\n");
        exit();
    }

    int n = write(fd, buf, length(buf));
    write(fd, empty, SIZE_BUFF - length(buf));
    write(fd, (char *)"\n", 1);
    if (n < 0)
    {
        printf(2, "Failed to write to the file.\n");
        close(fd);
        exit();
    }

    close(fd);

    exit();
}
