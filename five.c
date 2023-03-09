#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define HBLK "\e[0;90m"
#define reset "\e[0m"

#define NUM_OF_GRADES 10
#define THREAD_COUNT 11

pthread_barrier_t barrier;
pthread_mutex_t lock;

// global variables
int grades[NUM_OF_GRADES];
int total_grade = 0;
float total_bellcurve = 0;

void *read_grades(void *null)
{
    FILE *fptr;
    fptr = fopen("grades.txt", "r");

    // error opening file
    if (fptr == NULL)
    {
        printf("error opening file\n");
        exit(0);
    }

    // read one grade at a time
    for (int i=0; i<NUM_OF_GRADES; i++)
    {
        if (!feof(fptr))
        {
            // store in global array
            fscanf(fptr, "%d", &grades[i]);
        }
        else
        {
            printf("error\n");
            exit(0);
        }
    }

    pthread_barrier_wait(&barrier);

    // terminate thread
    pthread_exit(NULL);
}

void *save_bellcurve(void *g)
{
    FILE *fptr;
    fptr = fopen("grades.txt", "a");

    // error opening file
    if (fptr == NULL)
    {
        printf("error opening file\n");
        exit(0);
    }

    // wait for grades to be read
    pthread_barrier_wait(&barrier);

    // lock class so no other threads can access
    pthread_mutex_lock(&lock);

    // get argument as an int
    int grade = *((int *)g);
    float curve = grade * 1.5;

    // update global variable
    total_grade += grade;
    total_bellcurve += curve;

    // unlock class
    pthread_mutex_unlock(&lock);

    // append to fule
    fprintf(fptr, "%f\n", curve);

    // terminate thread
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[THREAD_COUNT];

    // read grades
    pthread_create(&threads[0], NULL, read_grades, NULL);

    // initialize barrier
    pthread_barrier_init(&barrier, NULL, 11);

    // create threads
    for (int i=1; i<THREAD_COUNT; i++)
    {
        pthread_create(&threads[i], NULL, save_bellcurve, &grades[i-1]);
    }

    // wait for threads to finish
    for (int i=0; i<THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // output
    printf("%stotal grade: %s%d\n", HBLK, reset, total_grade);
    printf("%saverage grade before curve: %s%f\n", HBLK, reset,(float)total_grade/NUM_OF_GRADES);
    printf("%saverage grade after curve: %s%f\n", HBLK, reset, (float)total_bellcurve/NUM_OF_GRADES);
}