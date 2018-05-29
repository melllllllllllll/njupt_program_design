#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IN_STR_LEN 50

void process(unsigned long num)
{
    unsigned long tmp = num;
    unsigned long mask = 1UL << (sizeof(unsigned long) * 8 - 1);
    char bin_str[sizeof(unsigned long) * 8 + 1];
    int offset = 0;

    printf("------------------\n");
    printf("bin: ");
    while(mask > 0)
    {
        sprintf(bin_str + offset, "%c", tmp & mask ? '1' : '0');
        mask >>= 1;
        offset++;
    }
    printf("%s", strchr(bin_str,'1'));
    printf("\n");
    printf("oct: %#lo\n", num);
    printf("dec: %ld\n", num);
    printf("hex: %#lx\n", num);
    printf("------------------\n");
}

int main(int argc, char *argv[])
{
    char str[IN_STR_LEN];
    unsigned long num;
    int base;
    char oncemore;

    do {
        printf("switch the base(2/8/10/16):\n");
        scanf("%d", &base);
        switch(base)
        {
            case 2:
                printf("input a number in base 2:\n");
                scanf("%s", str);
                num = strtoul(str, (char**)NULL, 2);
                break;
            case 8:
                printf("input a number in base 8:\n");
                scanf("%s", str);
                num = strtoul(str, (char**)NULL, 8);
                break;
            case 10:
                printf("input a number in base 10:\n");
                scanf("%s", str);
                num = strtoul(str, (char**)NULL, 10);
                break;
            case 16:
                printf("input a number in base 16:\n");
                scanf("%s", str);
                num = strtoul(str, (char**)NULL, 16);
                break;
            default:
                printf("wrong choice, you can have a retry.\n");
                break;
        }
        process(num);
        printf("once more(y or n)?\n");
        scanf(" %c", &oncemore);
    } while(oncemore == 'y');

    return 0;
}
