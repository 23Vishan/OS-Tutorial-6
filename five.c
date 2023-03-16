#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

#define HCYN "\e[0;96m"
#define HYEL "\e[0;93m"
#define reset "\e[0m"

#define SIZE 5

int total_sum = 0;
sem_t mutex;

// calculates factorial
void *factorial(void *fact)
{
    int num = *((int *)fact);

    int factorial = 1;
    
    // lock
    sem_wait(&mutex);

    // calculate factorial
    for (int i=1; i<=num; i++)
    {
        factorial *= i;
    }

    // update global array
    total_sum += factorial;

    // release lock
    sem_post(&mutex);

    // output
    printf("%sNumber: %s%d, %sFactorial: %s%d\n", HCYN, reset, num, HCYN, reset, factorial);

    // terminate thread
    pthread_exit(NULL);
}

int main()
{
    int child;
    FILE *fptr = fopen("numbers.txt", "w+");

    // create semaphore
    sem_init(&mutex, 0, 1);

    // get input
    for (int i=0; i<SIZE; i++)
    {
        int num;

        // prompt
        printf("%senter number %d: %s", HYEL, i+1, reset);
        scanf("%d", &num);

        // write to file
        fprintf(fptr, "%d\n", num);
    }
    printf("\n");

    // close file pointer
    fclose(fptr);

    // create child
    child = fork();

    // child process
    if (child == 0)
    {
        // holds threads id's
        pthread_t threads[SIZE];

        FILE *fptr_1 = fopen("numbers.txt", "r");
        FILE *fptr_2 = fopen("sum.txt", "w+");

        for (int i=0; i<SIZE; i++)
        {
            // read input
            int *num = (int*)malloc(sizeof(int));
            fscanf(fptr_1, "%d", num);

            // create thread
            pthread_create(&threads[i], NULL, factorial, (void *)num);
        }

        // wait for threads to finish
        for (int i=0; i<SIZE; i++)
        {
            pthread_join(threads[i], NULL);
        }

        // write to file
        fprintf(fptr_2, "%d\n", total_sum);
    }

    if (child > 0)
    {
        // wait for child to terminate
        waitpid(child, NULL, 0);

        FILE *fptr = fopen("sum.txt", "r");

        // read sum
        int sum;
        fscanf(fptr, "%d", &sum);

        // print output
        printf("\n%sTotal Sum: %s%d\n", HYEL, reset, sum);
    }
}