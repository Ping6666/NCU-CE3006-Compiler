%option noyywrap
%{
    #include "StackBaseMachine_A.tab.h"
%}
%%
\n      { /* do nothing */ }
[ \t]+  { /* do nothing */ }
^"add"  { return ADD; }
^"sub"  { return SUB; }
^"mul"  { return MUL; }
^"mod"  { return MOD; }
^"inc"  { return INC; }
^"dec"  { return DEC; }
^"load" { return LOAD; }
[0-9]+  { yylval.ival = atoi(yytext); return INUMBER; }
.       { /* do nothing */ }
%%
