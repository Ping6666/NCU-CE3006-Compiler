#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char tmp[1000];
    int index = 0;
    while (true)
    {
        char ch = getchar();
        if (ch == EOF)
        {
            break;
        }
        else if (ch == '\n')
        {
            continue;
        }
        else
        {
            tmp[index] = ch;
            index++;
        }
    }

    int pre = 0, id = 0;
    while (true)
    {
        if (id >= index)
        {
            break;
        }

        if ((tmp[id] - '0' >= 0) && (tmp[id] - '0' <= 9))
        {
            if (pre == 0)
            {
                printf("NUM ");
            }
            printf("%c", tmp[id]);
            pre = id + 1;
        }
        else
        {
            if (pre != 0)
            {
                pre = 0;
                printf("\n");
            }
            if (tmp[id] == '+')
            {
                printf("PLUS\n");
            }
            else if (tmp[id] == '-')
            {
                printf("MINUS\n");
            }
            else if (tmp[id] == '*')
            {
                printf("MUL\n");
            }
            else if (tmp[id] == '/')
            {
                printf("DIV\n");
            }
            else if (tmp[id] == '(')
            {
                printf("LPR\n");
            }
            else if (tmp[id] == ')')
            {
                printf("RPR\n");
            }
            // else ' ' or '\n'
        }
        id++;
    }

    return 0;
}