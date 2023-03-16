#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

#define HGRN "\e[0;92m"
#define HYEL "\e[0;93m"
#define HCYN "\e[0;96m"
#define reset "\e[0m"

#define BUFFER_SIZE 5
#define NUMBERS 10
#define THREADS 2
#define TRUE 1
#define FALSE 0

int buffer[5];
sem_t mutex;

void random_delay()
{
    // random int between 1 and 3
    int r = rand() %3 +1;

    // random delay
    sleep(r);
}

// produce data to buffer
void *producer(void *in)
{
    int *input = (int*)in;
    int index = 0;
    int found = FALSE;

    // until all numbers have been produced
    for (int i=0; i<NUMBERS; i++)
    {
        index = 0;
        found = FALSE;

        while (found == FALSE)
        {
            // find index of empty space in buffer
            for (index=0; index<BUFFER_SIZE; index++)
            {
                if (buffer[index] == 0)
                {
                    found = TRUE;
                    break;
                }
            }

            // if empty space was found
            if (found == TRUE)
            {
                // wait
                random_delay();
                
                // lock
                sem_wait(&mutex);

                // produce
                buffer[index] = input[i];

                // print
                printf("%sProduced:%s %d\n", HCYN, reset, input[i]);

                // release lock
                sem_post(&mutex);
            }
        }
    }

    // terminate thread
    pthread_exit(NULL);
}

// consume data from buffer
void *consumer()
{
    int index = 0;
    int found = FALSE;

    // until all numbers have been consumed
    for (int i=0; i<NUMBERS; i++)
    {
        index = 0;
        found = FALSE;

        while (found == FALSE)
        {
            // find index of non empty space in buffer
            for (index=0; index<BUFFER_SIZE; index++)
            {
                if (buffer[index] != 0)
                {
                    found = TRUE;
                    break;
                }
            }

            // if empty space was found
            if (found == TRUE)
            {
                // wait
                random_delay();
                
                // lock
                sem_wait(&mutex);

                // print
                printf("%sConsumed:%s %d\n", HYEL, reset, buffer[index]);

                // consume
                buffer[index] = 0;

                // release lock
                sem_post(&mutex);
            }
        }
    }

    // terminate thread
    pthread_exit(NULL);
}

int main()
{
    int input[NUMBERS];         // input
    pthread_t threads[THREADS]; // threads
    sem_init(&mutex, 0, 1);     // semaphore

    // initialize buffer
    for (int i=0; i<BUFFER_SIZE; i++)
    {
        buffer[i] = 0;
    }

    // get input
    for (int i=0; i<NUMBERS; i++)
    {
        printf("%senter number %d:%s ", HGRN, i+1, reset);
        scanf("%d", &input[i]);
    }
    printf("\n");

    // create thread
    pthread_create(&threads[0], NULL, producer, (void *)input);
    pthread_create(&threads[1], NULL, consumer, NULL);

    // wait for threads to finish
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    // print buffer
    printf("\n%sbuffer contents: %s", HGRN, reset);
    for(int i=0; i<BUFFER_SIZE; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");
}