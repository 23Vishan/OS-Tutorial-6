#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAG "\e[0;95m"
#define reset "\e[0m"

int main()
{
    // child processes
    int child_1 = fork();
    int child_2;

    // master process
    if (child_1 > 0)
    {   
        // create second child
        child_2 = fork();

        // create files
        FILE *fptr_1 = fopen("child1.txt", "w+");
        FILE *fptr_2 = fopen("child2.txt", "w+");

        // write to files
        fprintf(fptr_1, "child 1\n");
        fprintf(fptr_2, "child 2\n");

        // close file pointers
        fclose(fptr_1);
        fclose(fptr_2);
    }

    // child one
    if (child_1 == 0 && (child_2 > 0 || !child_2 ))
    {
        sleep(1); // wait one second

        FILE *fptr = fopen("child1.txt", "r"); // open file

        // read input
        char buf[10];
        fgets(buf, sizeof(buf), fptr);

        // print output
        printf("%sChild 1 Output: %s%s", MAG, buf, reset);
    }

    // child two
    if (child_2 == 0 && child_1 > 0)
    {
        sleep(1); // wait one second

        FILE *fptr = fopen("child2.txt", "r"); // open file

        // read input
        char buf[10];
        fgets(buf, sizeof(buf), fptr);

        // print output
        printf("%sChild 2 Output: %s%s", MAG, buf, reset);

    }

    // wait for child processes to finish
    waitpid(child_1, NULL, 0);
    waitpid(child_2, NULL, 0);
}