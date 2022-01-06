%{
    #include <stdio.h>
    int stackNumber[100];
    int stackPointer = 0;
    int yylex();
    void yyerror(const char *message);
%}
%union
{
    int ival;
}
%token  PUSH INVERSE INC DEC
%token  <ival>  INUMBER
%%
lines   : line  {
                    printf("%d\n", stackNumber[stackPointer - 1]);
                }
        ;
line    : expr line
        | expr
        ;
expr    : PUSH INUMBER  {
                            if(stackPointer >= 0)
                            {
                                stackNumber[stackPointer] = $2;
                                stackPointer++;
                            }
                        }
        | INVERSE       {
                            if(stackPointer >= 2)
                            {
                                int tmp = stackNumber[stackPointer - 2];
                                stackNumber[stackPointer - 2] = stackNumber[stackPointer - 1];
                                stackNumber[stackPointer - 1] = tmp;
                            }
                        }
        | INC           {
                            if(stackPointer >= 1)
                            {
                                stackNumber[stackPointer - 1] += 1;
                            }
                        }
        | DEC           {
                            if(stackPointer >= 1)
                            {
                                stackNumber[stackPointer - 1] -= 1;
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
