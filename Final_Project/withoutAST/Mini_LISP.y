%{
    #include <stdio.h>
    #include <string.h>
    #include <stdbool.h>
    struct define_name
    {
        char name[50];
        int index;      // decide value type
        bool bvalue;    // 1
        int ivalue;     // 2
    };
    int structarrlen = 0;
    struct define_name the_define_arr[50];
    int doCheck(char a[], int select, struct define_name *checkNow);
    void printfTE();
    void yyerror(const char *message);
    int yylex();
%}
%code requires
{
    struct Var
    {
        int index;          // decide val type
        bool bval;          // 1
        int ival;           // 2
        char cval[50];      // 3
        int result_plus;    // 4
        int result_multi;   // 5
        bool result_equal;  // 6
        bool result_and;    // 7
        bool result_or;     // 8
    };
}
%union
{
    struct Var tokenVar;
}
%token  LBC RBC
%token  PRINT_NUM PRINT_BOOL
%token  TOKEN_PLUS TOKEN_MINUS TOKEN_MULTIPLY TOKEN_DIVIDE
%token  TOKEN_GREATER TOKEN_SMALLER TOKEN_EQUAL
%token  MOD AND OR NOT DEFINE IF
%token  <tokenVar>  BOOL_VAL NUMBER ID
%type   <tokenVar>  exp exps variable
%type   <tokenVar>  num_op plus minus multiply divide modulus greater smaller equal
%type   <tokenVar>  logical_op and_op or_op not_op
%type   <tokenVar>  fun_exp fun_call fun_name
%type   <tokenVar>  if_exp test_exp then_exp else_exp
%%
// PROGRAM ::= STMT+
program     : stmts         {
                                // printf("HI\n");
                            }
            ;
stmts       : stmt stmts
            | {}
            ;
// STMT ::= EXP | DEF_STMT | PRINT_STMT
stmt        : exp
            | def_stmt
            | print_stmt
            ;
// PRINT_STMT ::= (print_num EXP) | (print_bool EXP)
print_stmt  : LBC PRINT_NUM exp RBC     {
                                            // tokenVar
                                            if ($3.index == 3)
                                            {
                                                struct define_name check_result;
                                                if (doCheck($3.cval, 2, &check_result) == 1)
                                                {
                                                    printf("%d\n", check_result.ivalue);
                                                }
                                                else
                                                {
                                                    printfTE();
                                                    return(0);
                                                }
                                            }
                                            else if ($3.index == 2) // int
                                            {
                                                printf("%d\n", $3.ival);
                                            }
                                            else
                                            {
                                                printfTE();
                                                return(0);
                                            }
                                        }
            | LBC PRINT_BOOL exp RBC    {
                                            // tokenVar
                                            if ($3.index == 3)
                                            {
                                                struct define_name check_result;
                                                if (doCheck($3.cval, 1, &check_result) == 1)
                                                {
                                                    if (check_result.bvalue)
                                                    {
                                                        printf("#t\n");
                                                    }
                                                    else
                                                    {
                                                        printf("#f\n");
                                                    }
                                                }
                                                else
                                                {
                                                    printfTE();
                                                    return(0);
                                                }
                                            }
                                            else if ($3.index == 1) // bool
                                            {
                                                if ($3.bval)
                                                {
                                                    printf("#t\n");
                                                }
                                                else
                                                {
                                                    printf("#f\n");
                                                }
                                            }
                                            else
                                            {
                                                printfTE();
                                                return(0);
                                            }
                                        }
            ;
// EXP ::= bool_val | number | VARIABLE | NUM_OP | LOGICAL_OP | FUN_EXP | FUN_CALL | IF_EXP
exp         : BOOL_VAL      {
                                // tokenVar
                                $$.index = 1;
                                $$.bval = $1.bval;
                            }
            | NUMBER        {
                                // tokenVar
                                $$.index = 2;
                                $$.ival = $1.ival;
                            }
            | variable      {
                                // tokenVar
                                /*
                                previous setting
                                */
                                // $$.index = 3;
                                // strncpy($$.cval, $1.cval, (sizeof($1.cval) / sizeof(char)));
                                /*
                                current setting
                                not sure if only change here (checking the type with 3 which is char)
                                can be work for all situations
                                */
                                struct define_name check_result;
                                if (doCheck($1.cval, 1, &check_result) == 1) // is bool
                                {
                                    $$.index = check_result.index;
                                    $$.bval = check_result.bvalue;
                                    $$.ival = check_result.ivalue;
                                    $$.result_and = check_result.bvalue;
                                    $$.result_or = check_result.bvalue;
                                    // $$.result_equal = true;
                                    $$.result_plus = check_result.ivalue;
                                    $$.result_multi = check_result.ivalue;
                                }
                                else if (doCheck($1.cval, 2, &check_result) == 1) // is int
                                {
                                    /*
                                    testing output stream
                                    */
                                    // printf("TEST:%s,%d\n", $1.cval, check_result.ivalue);
                                    $$.index = check_result.index;
                                    $$.bval = check_result.bvalue;
                                    $$.ival = check_result.ivalue;
                                    // $$.result_and = true;
                                    // $$.result_or = true;
                                    $$.result_equal = true;
                                    $$.result_plus = check_result.ivalue;
                                    $$.result_multi = check_result.ivalue;
                                }
                                else // should never walk to here
                                {
                                    printf("No match! at exp | variable\n");
                                }
                            }
            | num_op        {
                                // modify needed
                                // tokenVar
                                $$ = $1;
                            }
            | logical_op    {
                                // modify needed
                                // tokenVar
                                $$ = $1;
                            }
            | fun_exp       {
                                // modify needed
                                // tokenVar
                                $$ = $1;
                            }
            | fun_call      {
                                // modify needed
                                // tokenVar
                                $$ = $1;
                            }
            | if_exp        {
                                // modify needed, done
                                // tokenVar
                                $$ = $1;
                            }
            ;
exps        : exp exps  {
                            // tokenVar
                            $$ = $1;
                            if ($1.index == 1) // bool
                            {
                                $$.result_equal = false;
                                if ($1.ival == $2.ival)
                                {
                                    $$.result_equal = true;
                                }
                                $$.result_equal = $$.result_equal && $2.result_equal;
                                $$.result_and = $1.bval && $2.result_and;
                                $$.result_or = $1.bval || $2.result_or;
                            }
                            else // int, this shoud never be char*
                            {
                                $$.result_plus = $1.ival + $2.result_plus;
                                $$.result_multi = $1.ival * $2.result_multi;
                            }
                        }
            | exp       {
                            // tokenVar
                            $$ = $1;
                            if ($1.index = 1)
                            {
                                $$.result_and = $1.bval;
                                $$.result_or = $1.bval;
                                // $$.result_equal = true;
                                $$.result_plus = $1.ival;
                                $$.result_multi = $1.ival;
                            }
                            else if ($1.index == 2)
                            {
                                // $$.result_and = true;
                                // $$.result_or = true;
                                $$.result_equal = true;
                                $$.result_plus = $1.ival;
                                $$.result_multi = $1.ival;
                            }
                            else // should never walk to here
                            {
                                printf("No match! at exps | exp\n");
                            }
                        }
            ;
// NUM_OP ::= PLUS | MINUS | MULTIPLY | DIVIDE | MODULUS | GREATER | SMALLER | EQUAL
num_op      : plus      {
                            // tokenVar
                            $$.index = $1.index;
                            $$.ival = $1.ival;
                        }
            | minus     {
                            // tokenVar
                            $$.index = $1.index;
                            $$.ival = $1.ival;
                        }
            | multiply  {
                            // tokenVar
                            $$.index = $1.index;
                            $$.ival = $1.ival;
                        }
            | divide    {
                            // tokenVar
                            $$.index = $1.index;
                            $$.ival = $1.ival;
                        }
            | modulus   {
                            // tokenVar
                            $$.index = $1.index;
                            $$.ival = $1.ival;
                        }
            | greater   {
                            // tokenVar
                            $$.index = $1.index;
                            $$.bval = $1.bval;
                        }
            | smaller   {
                            // tokenVar
                            $$.index = $1.index;
                            $$.bval = $1.bval;
                        }
            | equal     {
                            // tokenVar
                            $$.index = $1.index;
                            $$.bval = $1.bval;
                        }
            ;
// PLUS ::= (+ EXP EXP+)
plus        : LBC TOKEN_PLUS exp exps RBC       {
                                                    // tokenVar
                                                    $$.index = 2;
                                                    $$.ival = $3.ival + $4.result_plus;
                                                }
            ;
// MINUS ::= (- EXP EXP)
minus       : LBC TOKEN_MINUS exp exp RBC       {
                                                    // tokenVar
                                                    $$.index = 2;
                                                    $$.ival = $3.ival - $4.ival;
                                                }
            ;
// MULTIPLY ::= (* EXP EXP+)
multiply    : LBC TOKEN_MULTIPLY exp exps RBC   {
                                                    // tokenVar
                                                    $$.index = 2;
                                                    $$.ival = $3.ival * $4.result_multi;
                                                }
            ;
// DIVIDE ::= (/ EXP EXP)
divide      : LBC TOKEN_DIVIDE exp exp RBC      {
                                                    // tokenVar
                                                    $$.index = 2;
                                                    $$.ival = $3.ival / $4.ival;
                                                }
            ;
// MODULUS ::= (mod EXP EXP)
modulus     : LBC MOD exp exp RBC               {
                                                    // tokenVar
                                                    $$.index = 2;
                                                    $$.ival = $3.ival % $4.ival;
                                                }
            ;
// GREATER ::= (> EXP EXP)
greater     : LBC TOKEN_GREATER exp exp RBC     {
                                                    // tokenVar
                                                    $$.index = 1;
                                                    $$.bval = false;
                                                    if ($3.ival > $4.ival)
                                                    {
                                                        $$.bval = true;
                                                    }
                                                }
            ;
// SMALLER ::= (< EXP EXP)
smaller     : LBC TOKEN_SMALLER exp exp RBC     {
                                                    // tokenVar
                                                    $$.index = 1;
                                                    $$.bval = false;
                                                    if ($3.ival < $4.ival)
                                                    {
                                                        $$.bval = true;
                                                    }
                                                }
            ;
// EQUAL ::= (= EXP EXP+)
equal       : LBC TOKEN_EQUAL exp exps RBC      {
                                                    // tokenVar
                                                    $$.index = 1;
                                                    $$.bval = false;
                                                    if ($3.ival == $4.ival)
                                                    {
                                                        $$.bval = true;
                                                    }
                                                    $$.bval = $$.bval && $4.result_equal;
                                                }
            ;
// LOGICAL_OP ::= AND_OP | OR_OP | NOT_OP
logical_op  : and_op    {
                            // tokenVar
                            $$.index = $1.index;
                            $$.bval = $1.bval;
                        }
            | or_op     {
                            // tokenVar
                            $$.index = $1.index;
                            $$.bval = $1.bval;
                        }
            | not_op    {
                            // tokenVar
                            $$.index = $1.index;
                            $$.bval = $1.bval;
                        }
            ;
// AND_OP ::= (and EXP EXP+)
and_op      : LBC AND exp exps RBC  {
                                        // tokenVar
                                        $$.index = 1;
                                        $$.bval = $3.bval && $4.result_and;
                                    }
            ;
// OR_OP ::= (or EXP EXP+)
or_op       : LBC OR exp exps RBC   {
                                        // tokenVar
                                        $$.index = 1;
                                        $$.bval = $3.bval || $4.result_or;
                                    }
            ;
// NOT_OP ::= (not EXP)
not_op      : LBC NOT exp RBC       {
                                        // tokenVar
                                        $$.index = 1;
                                        $$.bval = !($3.bval);
                                    }
            ;
// DEF_STMT ::= (define VARIABLE EXP)
def_stmt    : LBC DEFINE variable exp RBC   {
                                                // $$.index = $4.index;
                                                the_define_arr[structarrlen].index = $4.index;
                                                // strncpy($$.cval, $3.cval, (sizeof($3.cval) / sizeof(char)));
                                                strncpy(the_define_arr[structarrlen].name, $3.cval, (sizeof($3.cval) / sizeof(char)));
                                                // bool
                                                the_define_arr[structarrlen].bvalue = $4.bval;
                                                // $$.bval = $4.bval;
                                                // int
                                                the_define_arr[structarrlen].ivalue = $4.ival;
                                                // $$.ival = $4.ival;
                                                structarrlen += 1;
                                            }
            ;
// VARIABLE ::= id
variable    : ID    {
                        // tokenVar
                        $$ = $1;
                        strncpy($$.cval, $1.cval, (sizeof($1.cval) / sizeof(char)));
                    }
            ;
// FUN_EXP ::= (fun FUN_IDs FUN_BODY) // change fun to ID
// fun_exp     : LBC FUN fun_ids fun_body RBC
/*
If the fun_name can be other than FUN, we need to use ID to detect fun_name.
But to do this will lead to bison comile warning shift/reduce conflict [-Wconflicts-sr].
*/
fun_exp     : LBC ID fun_ids fun_body RBC   {
                                                // ID is fun_name, fun_ids is fun_inputs.
                                                // bind ID and fun_ids
                                            }
            ;
// FUN_IDs ::= (id*)
fun_ids     : LBC ids RBC   {
                                // store the fun_inputs
                            }
            ;
ids         : ID ids        {
                                // set the fun_inputs
                            }
            | {}
            ;
// FUN_BODY ::= EXP
fun_body    : exp           {
                                // store exp as string??
                            }
            ;
// FUN_CALL ::= (FUN_EXP PARAM*) | (FUN_NAME PARAM*)
fun_call    : LBC fun_exp params RBC    {
                                            // call the fun_exp now
                                        }
            | LBC fun_name params RBC   {
                                            // find the fun by fun_name then call
                                        }
            ;
// PARAM ::= EXP
params      : exp params    {
                                // params is fun_inputs
                            }
            | {}
            ;
// LAST_EXP ::= EXP // don't know what is this, pass for now
// FUN_NAME ::= id
fun_name    : ID    {
                        // tokenVar
                        $$ = $1;
                    }
            ;
// IF_EXP ::= (if TEST_EXP THEN_EXP ELSE_EXP)
if_exp      : LBC IF test_exp then_exp else_exp RBC {
                                                        // tokenVar
                                                        if ($3.index == 1 && $3.bval)
                                                        {
                                                            // not sure if only check bval will work
                                                            $$ = $4;
                                                        }
                                                        else
                                                        {
                                                            $$ = $5;
                                                        }
                                                    }
            ;
// TEST_EXP ::= EXP
test_exp    : exp   {
                        // tokenVar
                        $$ = $1;
                    }
            ;
// THEN_EXP ::= EXP
then_exp    : exp   {
                        // tokenVar
                        $$ = $1;
                    }
            ;
// ELSE_EXP ::= EXP
else_exp    : exp   {
                        // tokenVar
                        $$ = $1;
                    }
            ;
%%
int doCheck(char a[], int select, struct define_name *checkNow)
{
    for(int i = 0; i < structarrlen; i++)
    {
        if (strcmp(a, the_define_arr[i].name) == 0)
        {
            if (the_define_arr[i].index == select)
            {
                *checkNow = the_define_arr[i];
                return 1;
            }
        }
    }
    return 0;
}
void printfTE()
{
    printf("Type error!\n");
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
