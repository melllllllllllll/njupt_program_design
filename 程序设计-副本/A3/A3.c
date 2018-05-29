#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_STR_LEN 100
#define MAX_COL_LEN 100
#define INSERT_INIT_LEN 50
#define INSERT_INC 50
#define SEARCH_INIT_LEN 50
#define SEARCH_INC 50
#define SUB_INIT_LEN 50
#define SUB_INC 50
#define BUF 200

int is_save = 1;
char name[FILENAME_MAX];
FILE *fp;
char tmp_modify_name[L_tmpnam];
char tmp_origin_name[L_tmpnam];
FILE *tmp_modify;
FILE *tmp_origin;
char str[MAX_STR_LEN + 1];

void print();
void view();
void insert();
void delete();
void search();
void substitude();
void save();
void exit_();
void process(int row, int col, int just_go);
void to_origin();

int main(int argc, char *argv[])
{
    char function[20];
    char more;

    tmpnam(tmp_modify_name);
    tmpnam(tmp_origin_name);

    printf("input the file name to be edit:\n");
    scanf("%s", name);
    fp = fopen(name, "r+");
    if(fp == NULL)
    {
        fprintf(stderr, "oops, file not exit or the other unknow error!!!\n");
        exit(1);
    }
    tmp_modify = fopen(tmp_modify_name, "w+");
    if(tmp_modify == NULL)
    {
        fprintf(stderr, "oops!!!\n");
        fclose(fp);
        exit(1);
    }
    tmp_origin = fopen(tmp_origin_name, "w+");
    if(tmp_origin == NULL)
    {
        fprintf(stderr, "oops!!!\n");
        fclose(tmp_modify);
        fclose(fp);
        exit(1);
    }
    while(fgets(str, MAX_STR_LEN + 1, fp) != NULL)
    {
        fputs(str, tmp_origin);
    }
    rewind(tmp_origin);
    do {
        printf("select the function:\n");
        printf("\tview\n");
        printf("\tinsert\n");
        printf("\tdelete\n");
        printf("\tsearch\n");
        printf("\tsubstitude\n");
        printf("\tsave\n");
        printf("\texit\n");

        scanf("%s", function);
        if(!strcmp(function, "view")) {
            view();
        } else if(!strcmp(function, "insert")) {
            insert();
        } else if(!strcmp(function, "delete")) {
            delete();
        } else if(!strcmp(function, "search")) {
            search();
        } else if(!strcmp(function, "substitude")) {
            substitude();
        } else if(!strcmp(function, "save")) {
            save();
        } else if(!strcmp(function, "exit")) {
            exit_();
            return 0;
        } else {
            printf("wrong choice, you can have a retry.\n");
        }
        printf("more operation(y or n)?\n");
        scanf(" %c", &more);
    } while(more == 'y');

    exit_();
    return 0;
}

void print()
{
    int row = 0;
    int col;
    int nextline = 1;
    int is_first = 1;
    printf("      ");
    for(col = 0; col < MAX_COL_LEN; col++)
    {
        printf("%c", "0123456789"[col % 10]);
    }
    printf("\n");
    rewind(tmp_origin);
    while(fgets(str, MAX_STR_LEN + 1, tmp_origin) != NULL)
    {
        if(nextline == 1)
        {
            printf("%4d  ", row);
            row++;
            nextline = 0;
        }
        if(is_first)
        {
            printf("%s", str);
            is_first = 0;
        }
        else
        {
            printf("\n      ");
            printf("%s", str);
        }
        if(strchr(str, '\n'))
        {
            nextline = 1;
            is_first = 1;
        }
    }
    if(!strchr(str, '\n'))
    {
        printf("\n");
    }
}

void insert()
{
    int row;
    int col;
    size_t len = INSERT_INIT_LEN + 1;
    char buf[BUF];
    char *insert_str = (char*)malloc(sizeof(char) * len);
    char moreline;
    insert_str[0] = '\0';
    is_save = 0;

    print();
    printf("select where to insert(row col):\n");
    scanf("%d%d", &row, &col);

    do {
        printf("input what you want to insert:\n");
        getchar();
        fgets(buf, BUF, stdin);

        while(strlen(insert_str) + strlen(buf) + 1 > len)
        {
            len += INSERT_INC;
        }
        insert_str = (char*)realloc(insert_str, len);
        if(strlen(buf) != 1)
        {
            buf[strlen(buf) - 1] = '\0';
        }
        strcat(insert_str, buf);
        printf("insert more line(y or n)?\n");
        scanf("%c", &moreline);
    } while(moreline == 'y');

    rewind(tmp_modify);
    rewind(tmp_origin);
    process(row, col, 0);
    fputs(insert_str, tmp_modify);
    free(insert_str);
    while(fgets(str, MAX_STR_LEN + 1, tmp_origin) != NULL)
    {
        fputs(str, tmp_modify);
    }
    to_origin();
}

void delete()
{
    int row_from;
    int col_from;
    int row_to;
    int col_to;
    is_save = 0;

    print();
    printf("select where to start delete(row_form col_from):\n");
    scanf("%d%d", &row_from, &col_from);
    printf("select where to end(not include) delete(row_to col_to):\n");
    scanf("%d%d", &row_to, &col_to);

    fclose(tmp_modify);
    remove(tmp_modify_name);
    tmpnam(tmp_modify_name);
    tmp_modify = fopen(tmp_modify_name, "w+");

    rewind(tmp_origin);
    process(row_from, col_from, 0);
    rewind(tmp_origin);
    process(row_to, col_to, 1);
    while(fgets(str, MAX_STR_LEN + 1, tmp_origin) != NULL)
    {
        fputs(str, tmp_modify);
    }

    fclose(tmp_origin);
    remove(tmp_origin_name);
    tmpnam(tmp_origin_name);
    tmp_origin = fopen(tmp_origin_name, "w+");

    to_origin();
}

void search()
{
    int row = 0;
    size_t len = SEARCH_INIT_LEN + 1;
    char buf[BUF];
    char *search_str = (char*)malloc(sizeof(char) * len);
    char *pos;
    search_str[0] = '\0';

    printf("input what you want to search:\n");
    getchar();
    fgets(buf, BUF, stdin);
    while(strlen(buf) + 1 > len)
    {
        len += SEARCH_INC;
    }
    search_str = (char*)realloc(search_str, len);
    buf[strlen(buf) - 1] = '\0';
    strcat(search_str, buf);

    rewind(tmp_origin);
    while(fgets(buf, BUF, tmp_origin) != NULL)
    {
        pos = strstr(buf, search_str);
        if(pos != NULL)
        {
            printf("row: %d, col: %ld.\n", row, pos - buf);
        }
        row++;
    }
    free(search_str);
}

void substitude()
{
    char ori[BUF];
    char sub[BUF];
    size_t ori_len;
    size_t sub_len;
    size_t len = SUB_INIT_LEN + 1;
    char *out = (char*)malloc(sizeof(char) * len);
    char *pos;
    out[0] = '\0';
    is_save = 0;

    print();
    getchar();
    printf("input the original content:\n");
    fgets(ori, BUF, stdin);
    printf("input the substitude content:\n");
    fgets(sub, BUF, stdin);
    ori_len = strlen(ori);
    sub_len = strlen(sub);
    if(ori_len != 1)
    {
        ori[ori_len - 1] = '\0';
    }
    if(sub_len != 1)
    {
        sub[sub_len - 1] = '\0';
    }

    rewind(tmp_origin);
    while(fgets(str, MAX_STR_LEN + 1, tmp_origin) != NULL)
    {
    // solve the first sub problem
    // select substitude
        while(strlen(out) + strlen(str) + sub_len + 1 > len)
        {
            len += SUB_INC;
        }
        out = (char*)realloc(out, len);
        pos = strstr(str, ori);
        if(pos == NULL)
        {
            strcat(out, str);
        }
        else
        {
            *pos = '\0';
            strcat(out, str);
            strcat(out, sub);
            strcat(out, pos + ori_len - 1);
        }
    }

    fclose(tmp_origin);
    remove(tmp_origin_name);
    tmpnam(tmp_origin_name);
    tmp_origin = fopen(tmp_origin_name, "w+");
    fputs(out, tmp_origin);
}

void save()
{
    if(!is_save)
    {
        rewind(tmp_origin);
        fclose(fp);
        remove(name);
        fp = fopen(name, "w+");
        while(fgets(str, MAX_STR_LEN + 1, tmp_origin) != NULL)
        {
            fputs(str, fp);
        }
        is_save = 1;
    }
}

void exit_()
{
    char exit_not_save;
    if(is_save)
    {
        fclose(fp);
        fclose(tmp_modify);
        fclose(tmp_origin);
        remove(tmp_modify_name);
        remove(tmp_origin_name);
    }
    else
    {
        printf("exit without save(y or n)?\n");
        scanf(" %c", &exit_not_save);
        if(exit_not_save == 'n')
        {
            save();
            exit_();
        }
        printf("file is not save.\n");
        exit(0);
    }
}

void process(int row, int col, int just_go)
{
    if(just_go)
    {
        while(row > 0)
        {
            fgets(str, MAX_STR_LEN + 1, tmp_origin);
            row--;
        }
        while(col > 0)
        {
            fgetc(tmp_origin);
            col--; 
        }
    }
    else
    {
        while(row > 0)
        {
            fgets(str, MAX_STR_LEN + 1, tmp_origin);
            fputs(str, tmp_modify);
            row--;
        }
        while(col > 0)
        {
            fputc(fgetc(tmp_origin), tmp_modify);
            col--; 
        }
    }
}

void to_origin()
{
    rewind(tmp_modify);
    rewind(tmp_origin);
    while(fgets(str, MAX_STR_LEN + 1, tmp_modify) != NULL)
    {
        fputs(str, tmp_origin);
    }
}

void view()
{
    print();
}
