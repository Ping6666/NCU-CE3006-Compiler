%{
    #include <stdio.h>
    void sematicerror(int errorIndex);
    void yyerror (const char *message);
%}
%code requires
{
    struct Matrix
    {
        int i;
        int j;
    };
}
%union
{
    int ival;
    int loc;
    struct Matrix tokenMatrix;
}
%token  <loc> ADDSUB
%token  <loc> MUL
%token  COMMA TP
%token  LSBC RSBC
%token  LBC RBC
%token  <ival> NUM
%type   <tokenMatrix> matrix

%left ADDSUB
%left MUL
%left TP
%%
line    : matrix                    {
                                        printf("Accepted\n");
                                        return(0);
                                    }
        ;
matrix  : LSBC NUM COMMA NUM RSBC   { $$.i = $2;    $$.j = $4;   }
        | LBC matrix RBC            { $$.i = $2.i;  $$.j = $2.j; }
        | matrix TP                 { $$.i = $1.j;  $$.j = $1.i; }
        | matrix MUL matrix         {
                                        if ($1.j == $3.i)
                                        {
                                            $$.i = $1.i;
                                            $$.j = $3.j;
                                        }
                                        else
                                        {
                                            sematicerror($2);
                                            return(0);
                                        }
                                    }
        | matrix ADDSUB matrix      {
                                        if (($1.i == $3.i) && ($1.j == $3.j))
                                        {
                                            $$.i = $1.i;
                                            $$.j = $1.j;
                                        }
                                        else
                                        {
                                            sematicerror($2);
                                            return(0);
                                        }
                                    }
        ;
%%
void sematicerror(int errorIndex)
{
    printf("Semantic error on col %d\n", errorIndex);
}
void yyerror (const char *message)
{
    printf("%s\n", message);
}
int main(int argc, char *argv[])
{
    yyparse();
    return(0);
}
