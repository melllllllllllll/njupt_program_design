#include <stdio.h>
#include <string.h>

#define MESLEN 200

void encrypt()
{
    char mes[MESLEN + 1];
    char encrypt_mes[MESLEN + 1];
    char key;
    unsigned int i;
    char a[26] = "";

    printf("input the message:\n");
    scanf("%s", mes);
    printf("input the key(1 to 26):\n");
    scanf(" %c", &key);

    strcat(a, "abcdefghijklmnopqrstuvwxyz" + (key - '0'));
    strncat(a, "abcdefghijklmnopqrstuvwxyz", key - '0');

    for(i = 0; i < strlen(mes); i++)
    {
        encrypt_mes[i] = a[mes[i] - 'a'];
    }
    encrypt_mes[i] = '\0';
    printf("message is encrypted:\t%s\n", encrypt_mes);
}

void decrypt()
{
    char encrypt_mes[MESLEN + 1];
    char mes[MESLEN + 1];
    char key;
    unsigned int i;
    char a[26] = "";

    printf("input the encrypted message:\n");
    scanf("%s", encrypt_mes);
    printf("input the key:\n");
    scanf(" %c", &key);

    strcat(a, "abcdefghijklmnopqrstuvwxyz" + (key - '0'));
    strncat(a, "abcdefghijklmnopqrstuvwxyz", key - '0');

    for(i = 0; i < strlen(encrypt_mes); i++)
    {
        mes[i] = "abcdefghijklmnopqrstuvwxyz"[strchr(a, encrypt_mes[i]) - a];
    }
    mes[i] = '\0';
    printf("message is decrypted:\t%s\n", mes);
}

int main(int argc, char *argv[])
{
    char oncemore;
	int choice;

    do {
        printf("select the function\n");
        printf("\t1.encrypt\n");
        printf("\t2.decrypt\n");
        printf("\t3.exit\n");

        scanf("%d", &choice);
		switch(choice)
		{
			case 1:
				encrypt();
				break;
			case 2:
				decrypt();
				break;
			case 3:
				return 0;
			default:
				printf("wrong choice, you can have a retry.\n");
		}
        printf("once more(y or n)?\n");
        scanf(" %c", &oncemore);
    } while(oncemore == 'y');

    return 0;
}
