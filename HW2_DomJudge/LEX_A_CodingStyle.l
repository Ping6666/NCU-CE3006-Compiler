%{
    int tabCount = 0;
    void printTab()
    {
        for (int i = 0; i < tabCount; i++)
        {
            printf("\t");
        }
    }
%}
%%
^\{  { printTab(); tabCount++; ECHO; }
\{  { tabCount++; ECHO; }
^\}  { tabCount--; printTab(); ECHO; }
\}  { tabCount--; ECHO; }
^.  { printTab(); ECHO; }
%%
int main(int argc, char *argv[]) {
    yylex();
    return(0);
}
