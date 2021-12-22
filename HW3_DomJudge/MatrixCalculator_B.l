%option noyywrap
%{
    #include "MatrixCalculator_B.tab.h"
    int errorIndex = 0;
%}
%%
\n      { /* do nothing */ }
[ \t]+  { errorIndex += yyleng; }
"+"|"-" { errorIndex += yyleng; yylval.loc = errorIndex; return ADDSUB; }
"*"     { errorIndex += yyleng; yylval.loc = errorIndex; return MUL; }
","     { errorIndex += yyleng; return COMMA; }
"["     { errorIndex += yyleng; return LSBC; }  // Left Square BraCket
"]"     { errorIndex += yyleng; return RSBC; }  // Right Square BraCket
"("     { errorIndex += yyleng; return LBC; }   // Left BraCket
")"     { errorIndex += yyleng; return RBC; }   // Right BraCket
"^T"    { errorIndex += yyleng; return TP; }    // TransPose
[0-9]+  { errorIndex += yyleng; yylval.ival = atoi(yytext); return NUM; }
.       { /* do nothing */ }
%%
