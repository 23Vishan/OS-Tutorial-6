#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

#define HYEL "\e[0;93m"
#define HCYN "\e[0;96m"
#define reset "\e[0m"

#define SIZE 5

int moving_sum[SIZE];
sem_t mutex;

typedef struct Fact {
    int number;
    int index;
} Fact;

// calculates factorial
void *factorial(void *fact)
{
    Fact *f = fact;

    int factorial = 1;

    if (f->index > 0)
    {
        // infinte signal and wait loop until function is ready
        while (moving_sum[f->index-1] <= 0)
        {
            sem_wait(&mutex);
            sem_post(&mutex);
        }
    }
    
    // lock
    sem_wait(&mutex);

    // calculate factorial
    for (int i=1; i<= f->number; i++)
    {
        factorial *= i;
    }

    // update global array
    moving_sum[f->index] = factorial + moving_sum[f->index-1];

    // release lock
    sem_post(&mutex);

    // terminate thread
    pthread_exit(NULL);
}

int main()
{
    // allocate space
    Fact **f = (Fact**)malloc(sizeof(Fact)*SIZE);

    // holds threads id's
    pthread_t threads[SIZE];
    
    // create semaphore
    sem_init(&mutex, 0, 1);

    // initialize global array
    for (int i=0; i<SIZE; i++)
    {
        moving_sum[i] = 0;
    }

    // initialize global array
    for (int i=0; i<SIZE; i++)
    {
        f[i] = (Fact*)malloc(sizeof(Fact));

        printf("%senter number %d: %s", HYEL, i+1, reset);
        scanf("%d", &f[i]->number);

        f[i]->index=i;
        
        // create thread
        pthread_create(&threads[i], NULL, factorial, (void *)f[i]);
    }

    // wait for threads to finish
    for (int i=0; i<SIZE; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // print output
    printf("contents of moving_sum: ");
    for (int i=0; i<SIZE; i++)
    {
        printf("%s%d ", HCYN, moving_sum[i]);
    }
    printf("%s\n", reset);
}