#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

#define HYEL "\e[0;93m"
#define reset "\e[0m"

int main()
{
    int status;
    int child_1 = fork();

    // child process
    if (child_1 == 0)
    {
        // wait one second
        sleep(1);
        printf("%schild process%s\n", HYEL, reset);
    }
    // parent process
    else
    {
        // waits for child to terminate
        wait(&status); 

        printf("%sparent process%s\n", HYEL, reset);
        // check exit status
        if (status == -1)
        {
            printf("%sunsuccessful exit%s\n", HYEL, reset);
        }
        else
        {
            printf("%ssuccessful exit%s\n", HYEL, reset);
        }
    }
}