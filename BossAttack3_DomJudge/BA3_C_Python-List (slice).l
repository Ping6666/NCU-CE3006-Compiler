%option noyywrap
%{
    #include "C.tab.h"
%}
%%
\n          { /* do nothing */ }
[ \t]+      { /* do nothing */ }
"+"         { return ADDITION; }
"*"         { return MULTIPLY; }
","         { return COMMA; }
":"         { return COLON; }
"["         { return LBRACKET; }
"]"         { return RBRACKET; }
[-]?[0-9]+  { yylval.num = atoi(yytext); return DIGITS; }
.           { /* do nothing */ }
%%
