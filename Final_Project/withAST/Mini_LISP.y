%{
    /*
    [LexAndYaccTutorial.pdf](https://cse.iitkgp.ac.in/~bivasm/notes/LexAndYaccTutorial.pdf)
    In fact this is the default action and need not be specified. ( $$ = $1; )
    */
    #include "ast.h"
    ASTnode *root;
    int yylex();
    void yyerror(const char *message);
%}
%union
{
    bool bval;
    int ival;
    char *cval;
    ASTnode *nval;
}
%token  LBC RBC PRINT_NUM PRINT_BOOL
%token  TOKEN_PLUS TOKEN_MINUS TOKEN_MULTIPLY TOKEN_DIVIDE
%token  TOKEN_GREATER TOKEN_SMALLER TOKEN_EQUAL
%token  MOD AND OR NOT FUN DEFINE IF

%token  <bval>  BOOL_VAL
%token  <ival>  INT_VAL
%token  <cval>  STRING_VAL

%type   <nval>  stmt stmts print_stmt def_stmt
%type   <nval>  exp exps variable ids params
%type   <nval>  num_op plus minus multiply divide modulus greater smaller equal
%type   <nval>  logical_op and_op or_op not_op
%type   <nval>  fun_exp fun_ids fun_body fun_call fun_name fun_stmts fun_stmt
%type   <nval>  if_exp test_exp then_exp else_exp
%%
/*
start of grammar overview
*/
// PROGRAM      ::= STMT+
// STMT         ::= EXP | DEF_STMT | PRINT_STMT
// PRINT_STMT   ::= (print_num EXP) | (print_bool EXP)
// EXP          ::= bool_val | number | VARIABLE | NUM_OP | LOGICAL_OP | FUN_EXP | FUN_CALL | IF_EXP
// NUM_OP       ::= PLUS | MINUS | MULTIPLY | DIVIDE | MODULUS | GREATER | SMALLER | EQUAL
// PLUS         ::= (+ EXP EXP+)
// MINUS        ::= (- EXP EXP)
// MULTIPLY     ::= (* EXP EXP+)
// DIVIDE       ::= (/ EXP EXP)
// MODULUS      ::= (mod EXP EXP)
// GREATER      ::= (> EXP EXP)
// SMALLER      ::= (< EXP EXP)
// EQUAL        ::= (= EXP EXP+)
// LOGICAL_OP   ::= AND_OP | OR_OP | NOT_OP
// AND_OP       ::= (and EXP EXP+)
// OR_OP        ::= (or EXP EXP+)
// NOT_OP       ::= (not EXP)
// DEF_STMT     ::= (define VARIABLE EXP)
// VARIABLE     ::= id
// FUN_EXP      ::= (fun FUN_IDs FUN_BODY)
// FUN_IDs      ::= (id*)
// FUN_BODY     ::= EXP
/* Nested Function */
// FUN_BODY     ::= DEF_STMT* EXP
// FUN_CALL     ::= (FUN_EXP PARAM*) | (FUN_NAME PARAM*)
// PARAM        ::= EXP
// LAST_EXP     ::= EXP // don't know what is this, pass for now
// FUN_NAME     ::= id
// IF_EXP       ::= (if TEST_EXP THEN_EXP ELSE_EXP)
// TEST_EXP     ::= EXP
// THEN_EXP     ::= EXP
// ELSE_EXP     ::= EXP
/*
End of grammar overview
*/

program     : stmts                                 { root = $1; }
            ;
stmts       : stmt stmts                            { $$ = mallocaddnode(ast_root, $1, $2); }
            | stmt ;
stmt        : exp | def_stmt | print_stmt ;
print_stmt  : LBC PRINT_NUM exp RBC                 { $$ = mallocaddnode(ast_print_num, $3, NULL); }
            | LBC PRINT_BOOL exp RBC                { $$ = mallocaddnode(ast_print_bool, $3, NULL); }
            ;
exp         : BOOL_VAL                              { $$ = mallocnode(ast_bool, $1); }
            | INT_VAL                               { $$ = mallocnode(ast_num, $1); }
            | variable | num_op | logical_op
            | fun_exp | fun_call | if_exp ;
exps        : exp exps                              { $$ = mallocaddnode(ast_continue, $1, $2); }
            | exp ;
num_op      : plus | minus | multiply | divide
            | modulus | greater | smaller | equal ;
plus        : LBC TOKEN_PLUS exp exps RBC           { $$ = mallocaddnode(ast_plus, $3, $4); }
            ;
minus       : LBC TOKEN_MINUS exp exp RBC           { $$ = mallocaddnode(ast_minus, $3, $4); }
            ;
multiply    : LBC TOKEN_MULTIPLY exp exps RBC       { $$ = mallocaddnode(ast_multiply, $3, $4); }
            ;
divide      : LBC TOKEN_DIVIDE exp exp RBC          { $$ = mallocaddnode(ast_divide, $3, $4); }
            ;
modulus     : LBC MOD exp exp RBC                   { $$ = mallocaddnode(ast_mod, $3, $4); }
            ;
greater     : LBC TOKEN_GREATER exp exp RBC         { $$ = mallocaddnode(ast_greater, $3, $4); }
            ;
smaller     : LBC TOKEN_SMALLER exp exp RBC         { $$ = mallocaddnode(ast_smaller, $3, $4); }
            ;
equal       : LBC TOKEN_EQUAL exp exps RBC          { $$ = mallocaddnode(ast_equal, $3, $4); }
            ;
logical_op  : and_op | or_op | not_op ;
and_op      : LBC AND exp exps RBC                  { $$ = mallocaddnode(ast_and, $3, $4); }
            ;
or_op       : LBC OR exp exps RBC                   { $$ = mallocaddnode(ast_or, $3, $4); }
            ;
not_op      : LBC NOT exp RBC                       { $$ = mallocaddnode(ast_not, $3, NULL); }
            ;
def_stmt    : LBC DEFINE variable exp RBC           { $$ = mallocaddnode(ast_define, $3, $4); }
            ;
variable    : STRING_VAL                            { $$ = mallocnode(ast_id, std::string($1)); }
            ;
fun_exp     : LBC FUN fun_ids fun_body RBC          { $$ = mallocaddnode(ast_fun, $3, $4); }
            ;
fun_stmts   : fun_stmt fun_stmts                    { $$ = mallocaddnode(ast_fun_body, $1, $2); }
            | fun_stmt ;
fun_stmt    : exp | def_stmt ;
fun_ids     : LBC ids RBC                           { $$ = $2; }
            ;
ids         : variable ids                          { $$ = mallocaddnode(ast_ids, $1, $2); }
            |                                       { $$ = NULL; }
            ;
fun_body    : fun_stmts ;
fun_call    : LBC fun_exp params RBC                { $$ = mallocaddnode(ast_fun_call, $2, $3); }
            | LBC fun_name params RBC               { $$ = mallocaddnode(ast_fun_call, $2, $3); }
            ;
params      : exp params                            { $$ = mallocaddnode(ast_params, $1, $2); }
            |                                       { $$ = NULL; }
            ;
fun_name    : variable ;
if_exp      : LBC IF test_exp then_exp else_exp RBC { $$ = mallocaddnode(ast_if, $4, $5, $3); }
            ;
test_exp    : exp ;
then_exp    : exp ;
else_exp    : exp ;
%%
void yyerror (const char *message)
{
    printf("%s\n", message);
}
int main(int argc, char *argv[])
{
    yyparse();
    ASTworkhouse(root);
    return(0);
}
