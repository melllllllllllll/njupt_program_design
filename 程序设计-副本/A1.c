#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME_LEN 50
#define STUDENT_COUNT 50

struct student
{
    char *name;
    int score;
};

void sort(struct student *students, int count)
{
    int i;
    int j;
    struct student tmp;
    for(i = 1; i < count; i++)
    {
        j = i;
        tmp = students[j];
        while(j > 0 && tmp.score > students[j-1].score)
        {
            students[j] = students[j-1];
            j--;
        }
        students[j] = tmp;
    }
}

void print(struct student *students, int count)
{
    int i;
    for(i = 0; i < count; i++)
    {
        printf("%-50s%d\n", students[i].name, students[i].score);
    }
}

void freemem(struct student *students, int count)
{
    int i;
    for(i = 0; i < count; i++)
    {
        free(students[i].name);
    }
}

int main(int argc, char *argv[])
{
    struct student students[STUDENT_COUNT];
    char *name;
    int score;
    int count;
    int i;
    printf("How many students?\n");
    scanf("%d", &count);
    printf("Input the student score with format: name score\n");
    for(i = 0; i < count; i++)
    {
        name = malloc(sizeof(char) * NAME_LEN + 1);
        if(name == NULL)
        {
            fprintf(stderr, "oops");
            exit(1);
        }
        scanf("%s%d", name, &score);
        students[i].name = name;
        students[i].score = score;
    }

    sort(students, count);
    print(students, count);
    freemem(students, count);

    return 0;
}
