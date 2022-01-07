%{
    /*
    title #
    */
    void titlePrint(const char *input, int length_)
    {
        int title_state = 0;
        int counter = 0;
        for (int i = 0; i < length_; i++)
        {
            if (input[i] == '#')
            {
                counter += 1;
                title_state = 1;
            }
            else if(input[i] == ' ' && title_state == 1)
            {
                title_state = 0;
                break;
            }
        }
        if (title_state != 0)
        {
            return;
        }

        printf("<h%d>", counter);
        for(int i = counter + 1; i < length_-1  ; i++)
        {
            printf("%c", input[i]);
        }
        if (input[length_ - 1] != '\n' && (input[length_ - 2] != '\n' || input[length_ - 2] != '\r'))
        {
            printf("%c", input[length_ - 1]);
        }
        printf("</h%d>", counter);
    }
    /*
    line
    */
    void linePrint()
    {
        printf("<hr />");
    }
    /*
    pic
    */
    void picPrint(const char *input, int length_)
    {
        int pic_state = 0;
        int counter1 = 0; // point to first in []
        int len1 = 0; // len in []
        int counter2 = 0; // point to first in ()
        int len2 = 0; // len in ()
        for (int i = 0; i < length_; i++)
        {
            if (input[i] == '!' && pic_state == 0)
            {
                counter1 += 1;
                pic_state = 1;
            }
            else if(input[i] == '[' && pic_state == 1)
            {
                counter1 += 1;
                pic_state = 2;
            }
            else if(input[i] == ']' && pic_state == 2)
            {
                counter2 += 1;
                pic_state = 3;
            }
            else if(input[i] == '(' && pic_state == 3)
            {
                counter2 += 1;
                pic_state = 4;
            }
            else if(input[i] == ')' && pic_state == 4)
            {
                pic_state = 5;
                break;
            }
            else
            {
                if (pic_state==2)
                {
                    // in []
                    len1 += 1;
                }
                else if (pic_state==4)
                {
                    // in ()
                    len2 += 1;
                }
            }
        }
        if (pic_state != 5)
        {
            return;
        }

        printf("<img alt=\"");
        for(int i = counter1; i < counter1 + len1; i++)
        {
            printf("%c", input[i]);
        }
        printf("\" src=\"");
        for(int i = counter2 + counter1 + len1; i < counter2 + counter1 + len1 + len2; i++)
        {
            printf("%c", input[i]);
        }
        printf("\">");
    }
    /*
    box
    */
    void boxPrint(const char *input, int length_)
    {
        int box_state = 0;
        int select_state = 0;
        if (input[0] == '-' && input[1] == ' ' && box_state == 0)
        {
            box_state = 1;
        }
        if(input[2] == '[' && input[3] == 'X' && input[4] == ']' && box_state == 1)
        {
            select_state = 1;
            box_state = 2;
        }
        else if(input[2] == '[' && input[3] == 'x' && input[4] == ']' && box_state == 1)
        {
            select_state = 1;
            box_state = 2;
        }
        else if(input[2] == '[' && input[3] == ' ' && input[4] == ']' && box_state == 1)
        {
            select_state = 0;
            box_state = 2;
        }
        if (input[5] == ' ' && box_state==2)
        {
            box_state = 3;
        }
        if (box_state!=3)
        {
            return;
        }

        printf("<p>\n");
        if (select_state==1)
        {
            printf("\t<input checked=\"checked\" type=\"checkbox\" /> ");
        }
        else
        {
            printf("\t<input type=\"checkbox\" /> ");
        }
        for (int i=6;i<length_;i++)
        {
            printf("%c", input[i]);
        }
        if (input[length_-1]!='\n')
        {
            printf("\n");
        }
        printf("</p>");
    }
%}
%%
^[#]*" "(.)+                { titlePrint(yytext, yyleng); }
"---"                       { linePrint(); }
^"\!\["(.)+"\]\("(.)+"\)"   { picPrint(yytext, yyleng); }
^"\-"" \["[X|x| ]"\] "(.)+  { boxPrint(yytext, yyleng); }
%%
int main(int argc, char *argv[]) {
    yylex();
    return(0);
}