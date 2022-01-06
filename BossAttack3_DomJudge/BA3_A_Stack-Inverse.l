%option noyywrap
%{
    #include "A.tab.h"
%}
%%
\n          { /* do nothing */ }
[ \t]+      { /* do nothing */ }
^"push"     { return PUSH; }
^"inverse"  { return INVERSE; }
^"inc"      { return INC; }
^"dec"      { return DEC; }
[0-9]+      { yylval.ival = atoi(yytext); return INUMBER; }
.           { /* do nothing */ }
%%
