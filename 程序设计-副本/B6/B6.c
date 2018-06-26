#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_LEN_MAX 20
#define STU_CNT_MAX 50
#define USUAL_RATE 30
#define MID_RATE 30
#define END_RATE 40

struct student {
    char id[ID_LEN_MAX + 1];
    double usual;
    double mid;
    double end;
    double score;
    char grade;
};

struct class_ {
    int stu_cnt;
    double avg_usual;
    double avg_mid;
    double avg_end;
    double avg_score;
};

struct grade {
    int stu_cnt;
    int grade_a;
    int grade_b;
    int grade_c;
    int grade_d;
    int grade_e;
};

void print_stus(struct student *stus, int cnt);
void print_class(struct class_ *cls);
void print_grade(struct grade * grd);
void print_specific_grade(struct student *stus, int cnt);
int get_grade_cnt(struct grade *grd, char grade);

int main(int argc, char *argv[])
{
    struct student stus[STU_CNT_MAX];
    struct class_ cls;
    struct grade grd;
    int stu_cnt = 0;
    int i = 0;
    int operation;
    char oncemore;
    double usual;
    double mid;
    double end;
    double score;
    double all_usual = 0;
    double all_mid = 0;
    double all_end = 0;
    double all_score = 0;
    

    FILE *fp = fopen("note", "r");
    if(fp == NULL)
    {
        fprintf(stderr, "oops");
        exit(1);
    }
    fscanf(fp, "%d", &stu_cnt);
    grd.stu_cnt = stu_cnt;
    grd.grade_a = 0;
    grd.grade_b = 0;
    grd.grade_c = 0;
    grd.grade_d = 0;
    grd.grade_e = 0;
    for(i = 0; i < stu_cnt; i++)
    {
        fscanf(fp, "%s%lf%lf%lf", stus[i].id, &usual, &mid, &end);

        stus[i].usual = usual;
        stus[i].mid = mid;
        stus[i].end = end;
        score = stus[i].score = (usual * USUAL_RATE + mid * MID_RATE + end * END_RATE) / 100;

        if(score >= 90) {
            stus[i].grade = 'A';
            grd.grade_a += 1;
        } else if(score >= 80) {
            stus[i].grade = 'B';
            grd.grade_b += 1;
        } else if(score >= 70) {
            stus[i].grade = 'C';
            grd.grade_c += 1;
        } else if(score >= 60) {
            stus[i].grade = 'D';
            grd.grade_d += 1;
        } else {
            stus[i].grade = 'E';
            grd.grade_e += 1;
        }

        all_usual += usual;
        all_mid += mid;
        all_end += end;
        all_score += score;
    }

    cls.stu_cnt = stu_cnt;
    cls.avg_usual = all_usual / stu_cnt;
    cls.avg_mid = all_mid / stu_cnt;
    cls.avg_end = all_end / stu_cnt;
    cls.avg_score = all_score / stu_cnt;
 
    do {
        printf("select the operation(input the digit):\n");
        printf("\t1.students list\n");
        printf("\t2.class average\n");
        printf("\t3.grade statistic\n");
        printf("\t4.specific grade statistic\n");

        scanf("%d", &operation);
        switch(operation)
        {
            case 1:
                print_stus(stus, stu_cnt);
                break;
            case 2:
                print_class(&cls);
                break;
            case 3:
                print_grade(&grd);
                break;
            case 4:
                print_specific_grade(stus, stu_cnt);
                break;
            default:
                printf("wrong choice, you can have a retry.\n");
                break;
        }
        printf("more operaton(y or n)?\n");
        scanf(" %c", &oncemore);
    } while(oncemore == 'y');

    return 0;
}

void print_stus(struct student *stus, int cnt)
{
    int i;
    FILE *fp = fopen("stus", "w");
    if(fp == NULL)
    {
        fprintf(stderr, "oops");
        exit(1);
    }
    printf(">>>student list:\n");
    fprintf(fp, "%-20s%-20s%-20s%-20s%-20s%-20s\n", "id", "usual", "mid", "end", "score", "grade");
    fprintf(stdout, "%-20s%-20s%-20s%-20s%-20s%-20s\n", "id", "usual", "mid", "end", "score", "grade");

    for(i = 0; i < cnt; i++)
    {
        fprintf(fp, "%-20s%-20.2f%-20.2f%-20.2f%-20.2f%-20c\n", stus[i].id, stus[i].usual, stus[i].mid, stus[i].end, stus[i].score, stus[i].grade);
        fprintf(stdout, "%-20s%-20.2f%-20.2f%-20.2f%-20.2f%-20c\n", stus[i].id, stus[i].usual, stus[i].mid, stus[i].end, stus[i].score, stus[i].grade);
    }

    printf("-----------------------------------------------------\n");
    fclose(fp);
}

void print_class(struct class_ *cls)
{
    FILE *fp = fopen("avg_class", "w");
    if(fp == NULL)
    {
        fprintf(stderr, "oops");
        exit(1);
    }

    printf(">>>class average:\n");
    fprintf(fp, "%-20s%-20s%-20s%-20s%-20s\n", "stu_count", "avg_usual", "avg_mid", "avg_end", "avg_score");
    fprintf(stdout, "%-20s%-20s%-20s%-20s%-20s\n", "stu_count", "avg_usual", "avg_mid", "avg_end", "avg_score");

    fprintf(fp, "%-20d%-20.2f%-20.2f%-20.2f%-20.2f\n", cls -> stu_cnt, cls -> avg_usual, cls -> avg_mid, cls -> avg_end, cls -> avg_score);
    fprintf(stdout, "%-20d%-20.2f%-20.2f%-20.2f%-20.2f\n", cls -> stu_cnt, cls -> avg_usual, cls -> avg_mid, cls -> avg_end, cls -> avg_score);

    printf("-----------------------------------------------------\n");
    fclose(fp);
}

void print_grade(struct grade *grd)
{
    int i;
    FILE *fp = fopen("grade_class", "w");
    if(fp == NULL)
    {
        fprintf(stderr, "oops");
        exit(1);
    }

    printf(">>>grade statistic:\n");
    fprintf(fp, "%-20s%-20s%-20s\n", "grade", "count", "rate(%)");
    fprintf(stdout, "%-20s%-20s%-20s\n", "grade", "count", "rate(%)");

    for(i = 0; i < 5; i++)
    {
        fprintf(fp, "%-20c%-20d%-20.2f\n", "ABCDE"[i], get_grade_cnt(grd, "abcde"[i]), (double)get_grade_cnt(grd, "abcde"[i]) / grd -> stu_cnt * 100); 
        fprintf(stdout, "%-20c%-20d%-20.2f\n", "ABCDE"[i], get_grade_cnt(grd, "abcde"[i]), (double)get_grade_cnt(grd, "abcde"[i]) / grd -> stu_cnt * 100); 
    }

    printf("-----------------------------------------------------\n");
    fclose(fp);
}

int get_grade_cnt(struct grade *grd, char grade)
{
    switch(grade)
    {
        case 'a':
            return grd -> grade_a;
            break;
        case 'b':
            return grd -> grade_b;
            break;
        case 'c':
            return grd -> grade_c;
            break;
        case 'd':
            return grd -> grade_d;
            break;
        case 'e':
            return grd -> grade_e;
            break;
        default:
            fprintf(stderr, "wrong argument");
            exit(1);
            break;
    }
}

void print_specific_grade(struct student *stus, int cnt)
{
    char c;
    int i;

    do {
        printf("input the grade you want to search(ABCDE):\n");
        scanf(" %c", &c);
    } while(!strchr("ABCDE", c));

    printf(">>>specific grade %c statistic:\n", c);
    printf("%-20s%-20s\n", "id", "score");
    for(i = 0; i < cnt; i++)
    {
        if(stus[i].grade == c) {
            printf("%-20s%-20.2f\n", stus[i].id, stus[i].score);            
        }
    }
    printf("-----------------------------------------------------\n");
}
