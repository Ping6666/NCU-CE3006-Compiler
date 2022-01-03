%option noyywrap
%{
    #include "ast.h"
    #include "Mini_LISP.tab.h"
%}
separator   [ |\t|\n|\r]+
letter      [a-z]
digit       [0-9]

number      0|[1-9]{digit}*|-[1-9]{digit}*
id          {letter}({letter}|{digit}|"-")*
bool_val_t  #t
bool_val_f  #f
%%
{separator}     { /* do nothing */ }

"("             { return LBC; }
")"             { return RBC; }

"print-num"     { return PRINT_NUM; }
"print-bool"    { return PRINT_BOOL; }

"+"             { return TOKEN_PLUS; }
"-"             { return TOKEN_MINUS; }
"*"             { return TOKEN_MULTIPLY; }
"/"             { return TOKEN_DIVIDE; }
">"             { return TOKEN_GREATER; }
"<"             { return TOKEN_SMALLER; }
"="             { return TOKEN_EQUAL; }
"mod"           { return MOD; }

"and"           { return AND; }
"or"            { return OR; }
"not"           { return NOT; }

"fun"|"lambda"  { return FUN; }
"define"        { return DEFINE; }
"if"            { return IF; }

{bool_val_t}    {
                    yylval.bval = true;
                    return BOOL_VAL;
                }
{bool_val_f}    {
                    yylval.bval = false;
                    return BOOL_VAL;
                }
{number}        {
                    yylval.ival = atoi(yytext);
                    return INT_VAL;
                }
{id}            {
                    yylval.cval = yytext;
                    return STRING_VAL;
                }

.               { /* do nothing */ }
%%
