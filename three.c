#define _XOPEN_SOURCE 600
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define MAX_NAME_LENGTH 50
#define NUM_STUDENTS 5

#define CYN "\e[0;36m"
#define BLU "\e[0;34m"
#define reset "\e[0m"

typedef struct Student {
    int grade;
    int student_id;
    char name[MAX_NAME_LENGTH];
} Student;

void *bellcurve(void *s)
{
    // get argument as a student struct
    Student *student = s;

    // output
    printf("%sStudent ID: %s%d\n%sName: %s%s\n%sBellcurved Grade: %s%f\n\n", 
            BLU, reset, student->student_id, BLU, reset, student->name, BLU, reset, student->grade*1.5);
    
    // terminate thread
    pthread_exit(NULL);
}

int main()
{
    Student **students = malloc(sizeof(Student)*NUM_STUDENTS);
    pthread_t threads[NUM_STUDENTS];

    // get input
    for (int i=0; i<NUM_STUDENTS; i++)
    {
        students[i] = malloc(sizeof(Student));

        printf("%senter student name:%s ", CYN, reset);
        scanf("%s", students[i]->name);

        printf("%senter student id:%s ", CYN, reset);
        scanf("%d", &students[i]->student_id);

        printf("%senter student grade:%s ", CYN, reset);
        scanf("%d", &students[i]->grade);
        printf("\n");
    }
    printf("\n");

    // create threads
    for (int i=0; i<NUM_STUDENTS; i++)
    {
        pthread_create(&threads[i], NULL, bellcurve, (void *)students[i]);
    }

    // wait for threads to finish
    for (int i=0; i<NUM_STUDENTS; i++)
    {
        pthread_join(threads[i], NULL);
    }
}