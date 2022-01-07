%{
    #include <stdio.h>
    #include <stdlib.h>
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
    struct SliceArr
    {
        int value1;
        int value2;
        int stepnum;
    };
}
%union
{
    int num;
    struct IntArr intArr;
    struct SliceArr sliceArr;
}
%token  ADDITION MULTIPLY
%token  COMMA COLON
%token  LBRACKET RBRACKET
%token  <num>   DIGITS

%type   <sliceArr>  Slice
%type   <intArr>    Sum Term List ListItem
%type   <num>       MulDigit StartIndex EndIndex Step

%left   ADDITION
%left   MULTIPLY
%%
S           : Sum                           {
                                                int i;
                                                if($1.length == 0)
                                                {
                                                    printf("[]\n");
                                                }
                                                else if($1.length == 1)
                                                {
                                                    printf("[%d]\n", $1.value[0]);
                                                }
                                                else
                                                {
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
MulDigit    : MulDigit MULTIPLY DIGITS          {
                                                    $$ = $1 * $3;
                                                }
            | DIGITS ;
List        : LBRACKET ListItem RBRACKET Slice  { 
                                                    if ($4.value1 < 0) // start
                                                    {
                                                        $4.value1 += $2.length;
                                                    }
                                                    if ($4.value2 < 0) // end
                                                    {
                                                        $4.value2 += $2.length;
                                                    }
                                                    int i;
                                                    int counter = 0;
                                                    for(i = $4.value1; i < $2.length; i += $4.stepnum)
                                                    {
                                                        if(i >= $4.value1 && i < $4.value2)
                                                        {
                                                            $$.value[counter] = $2.value[i];
                                                            counter += 1;
                                                        }
                                                    }
                                                    $$.length = counter;
                                                }
            ;
Slice       : LBRACKET StartIndex COLON EndIndex RBRACKET               {
                                                                            $$.value1 = $2;
                                                                            $$.value2 = $4;
                                                                            $$.stepnum = 1;
                                                                        }
            | LBRACKET StartIndex COLON EndIndex COLON Step RBRACKET    {
                                                                            $$.value1 = $2;
                                                                            $$.value2 = $4;
                                                                            $$.stepnum = $6;
                                                                        }
            |                                                           {
                                                                            $$.value1 = 0;
                                                                            $$.value2 = 100000;  // larger than length
                                                                            $$.stepnum = 1;
                                                                        }
            ;
StartIndex  : DIGITS    {
                            $$ = $1;
                        }
            |           {
                            $$ = 0;
                        }
            ;
EndIndex    : DIGITS    {
                            $$ = $1;
                        }
            |           {
                            $$ = 100000;  // larger than length
                        }
            ;
Step        : DIGITS    {
                            $$ = $1;
                        }
            |           {
                            $$ = 1;
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
