#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define YEL "\e[0;33m"
#define reset "\e[0m"

#define STUDENTS 5

void *bellcurve(void *g)
{
    // get argument as an int
    int grade = *((int *)g);

    printf("%d %shas been bellcurved to%s %f\n", grade, YEL, reset, grade*1.5);
    pthread_exit(NULL);
}

int main()
{
    int grades[STUDENTS];
    pthread_t threads[STUDENTS];

    // get input
    for (int i=0; i<STUDENTS; i++)
    {
        printf("%senter the grade of student %d:%s ", YEL, i+1, reset);
        scanf("%d", &grades[i]);
    }
    printf("\n");

    // create threads
    for (int i=0; i<STUDENTS; i++)
    {
        pthread_create(&threads[i], NULL, bellcurve, &grades[i]);
    }

    // wait for threads to finish
    for (int i=0; i<STUDENTS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}