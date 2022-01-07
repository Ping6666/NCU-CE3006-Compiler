%option noyywrap
%{
    #include <stdbool.h>
    #include "Mini_LISP.tab.h"
%}
%%
\n                  { /* do nothing */ }
[ \t]+              { /* do nothing */ }
"("                 { return LBC; }         // Left BraCket
")"                 { return RBC; }         // Right BraCket
"print-num"         { return PRINT_NUM; }
"print-bool"        { return PRINT_BOOL; }
"+"                 { return TOKEN_PLUS; }
"-"                 { return TOKEN_MINUS; }
"*"                 { return TOKEN_MULTIPLY; }
"/"                 { return TOKEN_DIVIDE; }
">"                 { return TOKEN_GREATER; }
"<"                 { return TOKEN_SMALLER; }
"="                 { return TOKEN_EQUAL; }
"mod"               { return MOD; }
"and"               { return AND; }
"or"                { return OR; }
"not"               { return NOT; }
"define"            { return DEFINE; }
"if"                { return IF; }
"#t"                {
                        yylval.tokenVar.index = 1;
                        yylval.tokenVar.bval = true;
                        return BOOL_VAL;
                    }
"#f"                {
                        yylval.tokenVar.index = 1;
                        yylval.tokenVar.bval = false;
                        return BOOL_VAL;
                    }
(0|[-]?[1-9][0-9]*) {
                        yylval.tokenVar.index = 2;
                        yylval.tokenVar.ival = atoi(yytext);
                        return NUMBER;
                    }
[-|0-9|a-z]+        {
                        yylval.tokenVar.index = 3;
                        strncpy(yylval.tokenVar.cval, yytext, (sizeof(yytext) / sizeof(char)));
                        return ID;
                    }
.                   { /* do nothing */ }
%%
