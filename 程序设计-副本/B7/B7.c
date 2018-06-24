#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PHONE_LEN_MAX 20
#define USER_NAME_LEN_MAX 20
#define LOCATION_LEN_MAX 10
#define CALL_TYPE_LEN_MAX 20

int is_cal = 0;

void cal_pay();
void pay_query();
void record_query();

int main(int argc, char *argv[])
{
    char oncemore;
    int option;

    do {
        printf("select the option:\n");
        printf("\t1.calculate the pay\n");
        printf("\t2.query the personal pay\n");
        printf("\t3.query the personal record\n");
        printf("\t4.exit\n");
        scanf("%d", &option);

        switch(option)
        {
            case 1:
                cal_pay();
                break;
            case 2:
                pay_query();
                break;
            case 3:
                record_query(); 
                break;
            case 4:
                exit(0);
                break;
            default:
                printf("wrong option, you can have a retry.\n");
                break;
        }

        printf("want more operation(y or n)?");
        scanf(" %c", &oncemore);
    } while(oncemore == 'y');

    return 0;
}

void cal_pay()
{
    FILE *fp_hd;
    FILE *fp_fl;
    FILE *fp_fy;

    char hd_from_location[LOCATION_LEN_MAX];
    char hd_from_phone[PHONE_LEN_MAX];
    char hd_to_location[LOCATION_LEN_MAX];
    char hd_to_phone[PHONE_LEN_MAX];
    int hd_sec;

    struct fl{
        char *fl_location;
        double fl_price; 
        struct fl *next;
    };
    struct fl *fl_list;
    {
        struct fl *tmp = malloc(sizeof(struct fl));
        if(tmp == NULL)
        {
            fprintf(stderr, "oops!!!\n");
            exit(1);
        }
        fl_list = tmp;
    }
    char tmp_fl_location[LOCATION_LEN_MAX];
    double tmp_fl_price;
    struct fl *now;

    int long_distance = 0;
    int minutes;
    double cost;

    is_cal = 1;
    fp_hd = fopen("hd", "r");
    fp_fl = fopen("fl", "r");
    fp_fy = fopen("fy", "w");
    if(fp_hd == NULL || fp_fy == NULL || fp_fl == NULL)
    {
        fprintf(stderr, "oops!!!\n");
        exit(1);
    }

    now = fl_list;
    while(fscanf(fp_fl, "%s%lf", tmp_fl_location, &tmp_fl_price) != EOF)
    {
        char *tmp_location = malloc(LOCATION_LEN_MAX * sizeof(char));
        if(tmp_location == NULL)
        {
            fprintf(stderr, "oops!!!\n");
            exit(1);
        }
        strcpy(tmp_location, tmp_fl_location);

        struct fl *tmp = malloc(sizeof(struct fl));
        if(tmp == NULL)
        {
            fprintf(stderr, "oops!!!\n");
            exit(1);
        }
        tmp -> fl_location = tmp_location;
        tmp -> fl_price = tmp_fl_price;
        tmp -> next = NULL;

        now -> next = tmp;
        now = now -> next;
    }

    fprintf(stdout, ">>>>>>Here is communication list\n");
    fprintf(fp_fy, "%-20s%-20s%-20s\n", "caller", "call type", "cost");
    fprintf(stdout, "%-20s%-20s%-20s\n", "caller", "call type", "cost");
    while(fscanf(fp_hd, "%s%s%s%s%d", hd_from_location, hd_from_phone, hd_to_location, hd_to_phone, &hd_sec) != EOF)
    {
        now = fl_list;
        minutes = (hd_sec + 59) / 60;
        if(strcmp(hd_from_location, hd_to_location))
        {
            long_distance = 1;
            do {
                now = now -> next;
                if(strcmp(now -> fl_location, hd_to_location) == 0)
                {
                    break;
                }
            } while(now -> next != NULL);
            cost = (now -> fl_price) * minutes;
        }
        else
        {
            cost = 0.3;
            if(minutes > 3)
            {
                cost += 0.2 * (minutes - 3);
            }
        }
        fprintf(fp_fy, "%-20s%-20s%-20.2f\n", hd_from_phone, long_distance ? "long-distance" : "short-distance", cost);
        fprintf(stdout, "%-20s%-20s%-20.2f\n", hd_from_phone, long_distance ? "long-distance" : "short-distance", cost);
        long_distance = 0;
    }
    fprintf(stdout, "-------------------------------------------------------\n");

    now = fl_list;
    do {
        now = now -> next;
        free(now -> fl_location);
    } while(now -> next != NULL);
    now = fl_list;
    while(now != NULL)
    {
        struct fl *tmp = now;
        now = now -> next;
        free(tmp);
    }

    fclose(fp_hd);
    fclose(fp_fl);
    fclose(fp_fy);
}

void pay_query()
{
    FILE *fp_fy;
    FILE *fp_yh;

    char phone[PHONE_LEN_MAX];

    char yh_phone[PHONE_LEN_MAX];
    char yh_name[USER_NAME_LEN_MAX];

    char fy_phone[PHONE_LEN_MAX];
    char fy_type[CALL_TYPE_LEN_MAX];
    double short_cost = 0;
    double long_cost = 0;
    double cost = 0;


    if(is_cal == 0)
    {
        cal_pay();
    }

    fp_fy = fopen("fy", "r");
    fp_yh = fopen("yh", "r");
    if(fp_fy == NULL || fp_yh == NULL )
    {
        fprintf(stderr, "oops!!!\n");
        exit(1);
    }

    printf("input the phone number to be searched:\n");
    scanf("%s", phone);
    while(fscanf(fp_yh, "%s%s", yh_phone, yh_name) != EOF)
    {
        if(strcmp(phone, yh_phone) == 0)
            break;
    }
    if(strcmp(phone, yh_phone))
    {
        printf("user not found!!!\n");
        return;
    }

    fprintf(stdout, ">>>>>>User %s\'s call charge\n", yh_name);
    fprintf(stdout, "%-20s%-20s%-20s%-20s%-20s\n", "user name", "phone", "short-distance", "long-distance", "all cost");
    fseek(fp_fy, 61L, SEEK_SET);
    while(fscanf(fp_fy, "%s%s%lf", fy_phone, fy_type, &cost) != EOF)
    {
        if(strcmp(phone, fy_phone))
        {
            continue;
        }
        if(strcmp(fy_type, "long-distance") == 0)
        {
            long_cost += cost;
        }
        else
        {
            short_cost += cost;
        }
    }
    fprintf(stdout, "%-20s%-20s%-20.2f%-20.2f%-20.2f\n", yh_name, yh_phone, short_cost, long_cost, short_cost + long_cost);
    fprintf(stdout, "-------------------------------------------------------\n");

    fclose(fp_fy);
    fclose(fp_yh);
}

void record_query()
{
    char phone[PHONE_LEN_MAX];

    char hd_from_phone[PHONE_LEN_MAX];
    char hd_to_phone[PHONE_LEN_MAX];
    int hd_sec;

    char yh_phone[PHONE_LEN_MAX];
    char yh_name[USER_NAME_LEN_MAX];

    FILE *fp_hd = fopen("hd", "r");
    FILE *fp_yh = fopen("yh", "r");
    if(fp_hd == NULL || fp_yh == NULL )
    {
        fprintf(stderr, "oops!!!\n");
        exit(1);
    }

    printf("input the phone number to be searched:\n");
    scanf("%s", phone);
    while(fscanf(fp_yh, "%s%s", yh_phone, yh_name) != EOF)
    {
        if(strcmp(phone, yh_phone) == 0)
            break;
    }
    if(strcmp(phone, yh_phone))
    {
        printf("user not found!!!\n");
        return;
    }
    fprintf(stdout, ">>>>>>User %s\'s record list\n", yh_name);
    fprintf(stdout, "%-20s%-20s%-20s%-20s%-20s\n", "caller", "from_phone", "callee", "to_phone", "last time(second)");
    while(fscanf(fp_hd, "%*s%s%*s%s%d", hd_from_phone, hd_to_phone, &hd_sec) != EOF)
    {
        if(strcmp(phone, hd_from_phone) == 0 || strcmp(phone, hd_to_phone) == 0)
        {
            char caller[USER_NAME_LEN_MAX];
            char callee[USER_NAME_LEN_MAX];
            fseek(fp_yh, 0L, SEEK_SET);
            while(fscanf(fp_yh, "%s%s", yh_phone, caller) != EOF)
            {
                if(strcmp(hd_from_phone, yh_phone) == 0)
                    break;
            }
            if(strcmp(hd_from_phone, yh_phone))
            {
                strcpy(caller, "not found");
            }
            fseek(fp_yh, 0L, SEEK_SET);
            while(fscanf(fp_yh, "%s%s", yh_phone, callee) != EOF)
            {
                if(strcmp(hd_to_phone, yh_phone) == 0)
                    break;
            }
            if(strcmp(hd_to_phone, yh_phone))
            {
                strcpy(callee, "not found");
            }
            fprintf(stdout, "%-20s%-20s%-20s%-20s%-20d\n", caller, hd_from_phone, callee, hd_to_phone, hd_sec);
        }
    }
    fprintf(stdout, "-------------------------------------------------------\n");
    fclose(fp_hd);
    fclose(fp_yh);
}
