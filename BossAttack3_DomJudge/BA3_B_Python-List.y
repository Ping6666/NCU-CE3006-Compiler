%{
    #include <stdio.h>
    int yylex();
    void yyerror (const char *message);
%}
%code requires
{
    struct IntArr
    {
        int length;
        int value[100];
    };
}
%union
{
    int num;
    struct IntArr intArr;
}
%token  ADDITION MULTIPLY
%token  COMMA
%token  LBRACKET RBRACKET
%token  <num>   DIGITS

%type   <intArr>    Sum Term List ListItem
%type   <num>       MulDigit

%left   ADDITION
%left   MULTIPLY
%%
S           : Sum                           {
                                                int i;
                                                for(i = 0; i < $1.length; i++)
                                                {
                                                    if(i == 0)
                                                    {
                                                        printf("[%d,", $1.value[i]);
                                                    }
                                                    else if(i == $1.length - 1)
                                                    {
                                                        printf(" %d]\n", $1.value[i]);
                                                    }
                                                    else
                                                    {
                                                        printf(" %d,", $1.value[i]);
                                                    }
                                                }
                                            }
            ;
Sum         : Term ADDITION Sum             {
                                                $$ = $1;
                                                int basenum = $1.length;
                                                int i;
                                                for(i = 0; i < $3.length; i++)
                                                {
                                                    $$.value[ basenum + i ] = $3.value[i];
                                                }
                                                $$.length += $3.length;
                                            }
            | Term ;
Term        : List MULTIPLY MulDigit                    {
                                                            $$ = $1;
                                                            int i;
                                                            for(i = 0; i < $3; i++)
                                                            {
                                                                int j;
                                                                for(j = 0; j < $1.length ; j++)
                                                                {
                                                                    $$.value[ $1.length * i + j ] = $1.value[j];
                                                                }
                                                            }
                                                            $$.length = $1.length * $3;
                                                        }
            | MulDigit MULTIPLY List                    {
                                                            $$ = $3;
                                                            int i;
                                                            for(i = 0; i < $1; i++)
                                                            {
                                                                int j;
                                                                for(j = 0; j < $3.length ; j++)
                                                                {
                                                                    $$.value[ $3.length * i + j ] = $3.value[j];
                                                                }
                                                            }
                                                            $$.length = $3.length * $1;
                                                        }
            | MulDigit MULTIPLY List MULTIPLY MulDigit  {
                                                            $$ = $3;
                                                            int multinum = $1 * $5;
                                                            int i;
                                                            for(i = 0; i < multinum; i++)
                                                            {
                                                                int j;
                                                                for(j = 0; j < $3.length ; j++)
                                                                {
                                                                    $$.value[ $3.length * i + j ] = $3.value[j];
                                                                }
                                                            }
                                                            $$.length = $3.length * multinum;
                                                        }
            | List ;
MulDigit    : MulDigit MULTIPLY DIGITS      {
                                                $$ = $1 * $3;
                                            }
            | DIGITS ;
List        : LBRACKET ListItem RBRACKET    { 
                                                $$ = $2;
                                            }
            ;
ListItem    : DIGITS COMMA ListItem {
                                        $$ = $3;
                                        int i;
                                        for(i = $$.length - 1; i >= 0; i--)
                                        {
                                            $$.value[i + 1] = $$.value[i];
                                        }
                                        $$.value[0] = $1;
                                        $$.length += 1;
                                    }
            | DIGITS                {
                                        $$.length = 1;
                                        $$.value[0] = $1;
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
