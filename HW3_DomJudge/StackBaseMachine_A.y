%{
    #include <stdio.h>
    int stackNumber[100];
    int stackPointer = 0;
    int moveValid = 1;
    void yyerror(const char *message);
%}
%union
{
    int ival;
}
%token  ADD SUB MUL MOD LOAD
%token  INC DEC
%token  <ival>  INUMBER
%type   <ival>  expr
%%
lines : line    {
                    if (moveValid == 1 && stackPointer == 1)
                    {
                        printf("%d\n", stackNumber[stackPointer - 1]);
                    }
                    else
                    {
                        printf("Invalid format\n");
                    }
                }
    ;
line : expr line
    | expr
    ;
expr : LOAD INUMBER {
                        if(stackPointer >= 0)
                        {
                            stackNumber[stackPointer] = $2;
                            stackPointer++;
                        }
                        else
                        {
                            moveValid = 0;
                        }
                        
                    }
    | ADD   {
                if(stackPointer >= 2)
                {
                    stackNumber[stackPointer - 2] = stackNumber[stackPointer - 1] + stackNumber[stackPointer - 2];
                    stackPointer--;
                }
                else
                {
                    moveValid = 0;
                }
            }
    | SUB   {
                if(stackPointer >= 2)
                {
                    stackNumber[stackPointer - 2] = stackNumber[stackPointer - 1] - stackNumber[stackPointer - 2];
                    stackPointer--;
                }
                else
                {
                    moveValid = 0;
                }
            }
    | MUL   {
                if(stackPointer >= 2)
                {
                    stackNumber[stackPointer - 2] = stackNumber[stackPointer - 1] * stackNumber[stackPointer - 2];
                    stackPointer--;
                }
                else
                {
                    moveValid = 0;
                }
            }
    | MOD   {
                if(stackPointer >= 2)
                {
                    stackNumber[stackPointer - 2] = stackNumber[stackPointer - 1] % stackNumber[stackPointer - 2];
                    stackPointer--;
                }
                else
                {
                    moveValid = 0;
                }
            }
    | INC   {
                if(stackPointer >= 1)
                {
                    stackNumber[stackPointer - 1] += 1;
                }
                else
                {
                    moveValid = 0;
                }
            }
    | DEC   {
                if(stackPointer >= 1)
                {
                    stackNumber[stackPointer - 1] -= 1;
                }
                else
                {
                    moveValid = 0;
                }
            }
    ;
%%
void yyerror (const char *message)
{
    printf("%s\n", message);
}
int main(int argc, char *argv[])
{
    yyparse();
    return(0);
}
