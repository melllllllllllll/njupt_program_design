#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 50

int main(int argc, char *argv[])
{
    int len;
    int test_count = 0;
    int win_count = 0;
    char *str;
    char *in;
    int i;
    char oncemore;

    srand(time(NULL));

    do {
        len = rand() % MAX_LEN;
        str = malloc(sizeof(char) * len + 1);
        in = malloc(sizeof(char) * len + 1);
        if(str == NULL || in == NULL)
        {
            fprintf(stderr, "oops");
            exit(1);
        }
        for(i = 0; i < len; i++)
        {
            str[i] = "abcdefghijklmnopqrstuvwxyz"[rand() % 26];
        }
        str[i] = '\0';
        printf("length is %d, string as follow\n%s\n", len, str);
        printf("let's get a test, input what you get\n");
        scanf("%s", in);
        test_count++;
        win_count += !strcmp(in, str) ? 1 : 0;
        free(str);
        free(in);
        printf("once more(y/n)?\n");
        scanf(" %c", &oncemore);
    } while (oncemore == 'y');

    printf("We have %d test%s, you win %d time%s.\n", test_count, test_count > 1 ? "s" : "", win_count, win_count > 1 ? "s" : ""); 
    printf("Win rate is %.2f%%.\n", (double)win_count / (double)test_count * 100);
    return 0;
}
