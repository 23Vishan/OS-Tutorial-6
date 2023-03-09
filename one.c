#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAG "\e[0;95m"
#define reset "\e[0m"

void wait()
{
    // random sleep duration from 1 to 3
    int random_time = rand() %5 +1;
    sleep(random_time);
}

void* hello_world(void *null)
{
    wait();
    printf("%shello world%s\n", MAG, reset);
    pthread_exit(NULL);
}

void* goodbye(void *null)
{
    wait();
    printf("%sgoodbye%s\n", MAG, reset);
    pthread_exit(NULL);
}

int main()
{
    // holds thread id's
    pthread_t t1, t2;

    // create threads
    pthread_create(&t1, NULL, hello_world, NULL);
    pthread_create(&t2, NULL, goodbye, NULL);

    // waits for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}