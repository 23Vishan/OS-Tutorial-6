#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_STUDENTS 10

#define HYEL "\e[0;93m"
#define HGRN "\e[0;92m"
#define reset "\e[0m"

int total_grade = 0;
pthread_mutex_t lock;

void *class_total(void *g)
{
    // lock class so no other threads can access
    pthread_mutex_lock(&lock);

    // get argument as an int
    int grade = *((int *)g);

    // update global variable
    total_grade += grade;

    // unlock class
    pthread_mutex_unlock(&lock);

    // terminate thread
    pthread_exit(NULL);
}

int main()
{
    int grades[NUM_STUDENTS];
    pthread_t threads[NUM_STUDENTS];

    // get input
    for (int i=0; i<NUM_STUDENTS; i++)
    {
        printf("%senter the grade of student %d:%s ", HGRN, i+1, reset);
        scanf("%d", &grades[i]);
    }

    // create threads
    for (int i=0; i<NUM_STUDENTS; i++)
    {
        pthread_create(&threads[i], NULL, class_total, &grades[i]);
    }

    // wait for threads to finish
    for (int i=0; i<NUM_STUDENTS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // destroy lock, no longer needed
    pthread_mutex_destroy(&lock);

    // print result
    printf("\n%stotal grade: %s%d\n", HYEL, reset, total_grade);
}