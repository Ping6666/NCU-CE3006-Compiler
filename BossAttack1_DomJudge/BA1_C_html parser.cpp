#include <iostream>
#include <vector>

// DEBUG
// bool print_check = true;
bool print_check = false;

/*
Recursive-Decent-Parsing:
Since need to fit the question needed, therefore try to use many function and if / else.
*/
// <div><span>text in span<div></span></div>
// htmlElement htmlContent htmlContent htmlElement  htmlContent htmlChardata
/*
1 htmlDocument -> htmlElement htmlDocument $
2 htmlDocument -> λ

3 htmlElement -> TAG_OPEN TAG_NAME htmlAttributeList TAG_CLOSE htmlContent TAG_OPEN_SLASH TAG_NAME TAG_CLOSE

4 htmlContent -> htmlChardata htmlContent
5 htmlContent -> htmlElement htmlContent
6 htmlContent -> λ

7 htmlAttributeList -> htmlAttribute htmlAttributeList
8 htmlAttributeList -> λ

9 htmlAttribute -> TAG_NAME TAG_EQUALS attribute

10 htmlChardata -> HTML_TEXT

11 attribute -> DOUBLE_QUOTE_STRING
12 attribute -> SINGLE_QUOTE_STRING
*/

struct AnsForm
{
    std::string TerminalName;

    AnsForm(std::string a)
    {
        this->TerminalName = a;
    }
};

// global variable
std::string tmp;
std::vector<AnsForm> ansForm;

void printVector()
{
    for (int i = 0; i < (int)ansForm.size(); i++)
    {
        std::cout << ansForm[i].TerminalName << "\n";
    }
    return;
}

void stringManipulate()
{
    bool set = false;
    for (int i = 0; i < (int)tmp.length(); i++)
    {
        if (set)
        {
            if (tmp[i] == ' ')
            {
                tmp.erase(tmp.begin() + i);
                i--;
            }
            else
            {
                set = false;
            }
        }
        else
        {
            if (tmp[i] == ' ')
            {
                set = true;
            }
        }
    }
    return;
}

// Productions
int GET__htmlDocument(int index);
int GET__htmlElement(int index);
int GET__htmlContent(int index, bool second = false);
int GET__htmlAttributeList(int index, bool second = false);
int GET__htmlAttribute(int index);
int GET__htmlChardata(int index);
int GET__attribute(int index);

// Terminal: Regular Expression
int GET_TAG_NAME(int index, bool first = true);                 // [a-z|A-Z|0-9]+
int GET_HTML_TEXT(int index, bool first = true);                // [^<]+
int GET_HTML_DOUBLE_QUOTE_STRING(int index, bool first = true); // "[^"<]*"
int GET_HTML_SINGLE_QUOTE_STRING(int index, bool first = true); // '[^'<]*'
int GET_TAG_OPEN(int index);                                    // <
int GET_TAG_CLOSE(int index);                                   // >
int GET_TAG_OPEN_SLASH(int index);                              // </
int GET_TAG_EQUALS(int index);                                  // =

int main()
{
    tmp.clear();
    while (true)
    {
        char ch = getchar();
        if (ch == EOF)
        {
            break;
        }
        else if (ch == '\n')
        {
            tmp += "\0";
        }
        else
        {
            tmp += ch;
        }
    }
    stringManipulate();
    // tmp += '\0'; // this is no need now

    if (print_check)
        std::cout << tmp << " " << tmp.length() << "\n";

    int result_ = GET__htmlDocument(0);

    if (result_ >= 0)
    {
        ansForm.push_back(AnsForm("htmlDocument"));
        printVector();
        std::cout << "valid\n";
    }
    else
    {
        printVector();
        std::cout << "invalid\n";
    }

    return 0;
}

/*
Productions
*/

int GET__htmlDocument(int index)
{
    if (print_check)
        std::cout << "GET__htmlDocument " << index << '\n';

    int index_ = GET__htmlElement(index);
    index += index_; // this shoud not do first, but it is ok to do first here
    if (index_ < 0)
    {
        return -1;
    }
    else if (index >= (int)tmp.length() - 1)
    {
        if (index_ > 0)
        {
            ansForm.push_back(AnsForm("htmlElement"));
        }
        return 0; // success
    }
    else if (index_ == 0)
    {
        ansForm.push_back(AnsForm("htmlElement"));
        return 0;
    }
    return GET__htmlDocument(index);
}

int GET__htmlElement(int index)
{
    if (print_check)
        std::cout << "GET__htmlElement " << index << '\n';

    int index_1 = GET_TAG_OPEN(index);
    if (index_1 < 0)
    {
        return 0; // this is diff. (maybe is 0)
    }
    index += index_1;
    int index_2 = GET_TAG_NAME(index);
    if (index_2 < 0)
    {
        return -1;
    }
    index += index_2;
    int index_3 = GET__htmlAttributeList(index);
    if (index_3 < 0)
    {
        return -1;
    }
    if (index_3 > 0)
    {
        ansForm.push_back(AnsForm("htmlAttributeList"));
    }
    index += index_3;
    int index_4 = GET_TAG_CLOSE(index);
    if (index_4 < 0)
    {
        return -1;
    }
    index += index_4;
    int index_5 = GET__htmlContent(index);
    if (index_5 < 0)
    {
        return -1;
    }
    if (index_5 > 0)
    {
        ansForm.push_back(AnsForm("htmlContent"));
    }
    index += index_5;
    int index_6 = GET_TAG_OPEN_SLASH(index);
    if (index_6 < 0)
    {
        return -1;
    }
    index += index_6;
    int index_7 = GET_TAG_NAME(index);
    if (index_7 < 0)
    {
        return -1;
    }
    index += index_7;
    int index_8 = GET_TAG_CLOSE(index);
    if (index_8 < 0)
    {
        return -1;
    }
    index += index_8;
    return index_1 + index_2 + index_3 + index_4 + index_5 + index_6 + index_7 + index_8;
}

// difficult func.
int GET__htmlContent(int index, bool second)
{
    if (print_check)
        std::cout << "GET__htmlContent " << index << '\n';

    int index_1 = GET__htmlChardata(index);
    if (index_1 < 0)
    {
        int index_1_1 = GET__htmlElement(index);
        if (index_1_1 < 0)
        {
            return 0; // this is diff. (maybe is 0), this is correct
        }
        if (index_1 == 0 || index_1_1 == 0)
            return 0; // the key factor for the wrong token seq.
        index += index_1_1;
        if (index_1_1 > 0)
        {
            ansForm.push_back(AnsForm("htmlElement"));
        }
        int index_1_2 = GET__htmlContent(index, true);
        if (index_1_2 < 0)
        {
            return -1; // this is diff. (maybe is 0)
        }
        index += index_1_2;
        if (index_1_1 + index_1_2 == 0)
            return -1;
        if (index_1_2 > 0)
        {
            ansForm.push_back(AnsForm("htmlContent"));
        }
        return index_1_1 + index_1_2;
    }
    else
    {
        index += index_1;
        if (index_1 > 0)
        {
            if (ansForm.size() == 0)
            {
                ansForm.push_back(AnsForm("htmlCharData"));
            }
            else if (ansForm.size() > 0 && ansForm.back().TerminalName.compare("htmlCharData") != 0)
            {
                ansForm.push_back(AnsForm("htmlCharData"));
            }
        }
        int index_2_1 = GET__htmlContent(index, true);
        if (index_2_1 < 0)
        {
            return -1; // this is diff. (maybe is 0)
        }
        index += index_2_1;
        if (index_2_1 > 0 && !second)
        {
            ansForm.push_back(AnsForm("htmlContent"));
        }
        if (index_1 + index_2_1 == 0)
            return -1;
        return index_1 + index_2_1;
    }
    return -1; // this is λ (0), this is diff. (maybe is 0)
}

int GET__htmlAttributeList(int index, bool second)
{
    if (print_check)
        std::cout << "GET__htmlAttributeList " << index << '\n';

    int index_ = GET__htmlAttribute(index);
    index += index_; // this shoud not do first, but it is ok to do first here
    if (index_ < 0)
    {
        return 0; // this is λ (0)
    }
    if (index_ > 0)
    {
        ansForm.push_back(AnsForm("htmlAttribute"));
    }
    int index_1 = GET__htmlAttributeList(index);
    /*
    the main problem of all may be here
    */
    if (index_1 < 0)
    {
        return -1; // this is diff. (maybe is 0)
    }
    index += index_1;
    if (index_1 > 0)
    {
        ansForm.push_back(AnsForm("htmlAttributeList"));
    }
    return index_ + index_1;
}

int GET__htmlAttribute(int index)
{
    if (print_check)
        std::cout << "GET__htmlAttribute " << index << '\n';

    int index_1 = GET_TAG_NAME(index);
    if (index_1 <= 0)
    {
        return -1; // this is diff. (maybe is 0)
    }
    index += index_1;
    int index_2 = GET_TAG_EQUALS(index);
    if (index_2 < 0)
    {
        return -1;
    }
    index += index_2;
    int index_3 = GET__attribute(index);
    if (index_3 < 0)
    {
        return -1;
    }
    index += index_3;
    if (index_3 > 0)
    {
        ansForm.push_back(AnsForm("attribute"));
    }
    return index_1 + index_2 + index_3;
}

int GET__htmlChardata(int index)
{
    if (print_check)
        std::cout << "GET__htmlChardata " << index << '\n';

    int index_1 = GET_HTML_TEXT(index);
    if (index_1 <= 0)
    {
        return -1; // this is diff. (maybe is 0)
    }
    index += index_1;
    return index_1;
}

int GET__attribute(int index)
{
    if (print_check)
        std::cout << "GET__attribute " << index << '\n';

    int index_1 = GET_HTML_DOUBLE_QUOTE_STRING(index);
    if (index_1 <= 0)
    {
        int index_2 = GET_HTML_SINGLE_QUOTE_STRING(index);
        if (index_2 <= 0)
        {
            return -1; // this is fail
        }
        return index_2;
    }
    else
    {
        ansForm.push_back(AnsForm("attribute"));
        return index_1;
    }
}

/*
Terminal: Regular Expression
*/

// [a-z|A-Z|0-9]+
int GET_TAG_NAME(int index, bool first)
{
    if (print_check)
        std::cout << "GET_TAG_NAME " << index << '\n';

    if (index >= (int)tmp.length())
        return -1; // out of range

    if (first) // first time call GET_TAG_NAME
    {
        if (((tmp[index] >= 'a') && (tmp[index] <= 'z')) || ((tmp[index] >= 'A') && (tmp[index] <= 'Z')) || ((tmp[index] >= '0') && (tmp[index] <= '9')))
        {
            int index_ = GET_TAG_NAME(index + 1, false);
            if (index_ < 0)
            {
                return -1;
            }
            return index_ + 1;
        }
        else if (tmp[index] == ' ')
        {
            int index_ = GET_TAG_NAME(index + 1, true);
            if (index_ < 0)
            {
                return -1;
            }
            return index_ + 1;
        }
        return -1;
    }
    else // second time call GET_TAG_NAME
    {
        if (((tmp[index] >= 'a') && (tmp[index] <= 'z')) || ((tmp[index] >= 'A') && (tmp[index] <= 'Z')) || ((tmp[index] >= '0') && (tmp[index] <= '9')))
        {
            int index_ = GET_TAG_NAME(index + 1, false);
            if (index_ < 0)
            {
                return -1;
            }
            return index_ + 1;
        }
        else if ((tmp[index] == '\n') || (tmp[index] == '\0') || (tmp[index] == ' ') || (tmp[index] == '<') || (tmp[index] == '>') || (tmp[index] == '='))
        {
            return 0; // not include this char (+0) (????)
        }
        return -1;
    }
}

// [^<]+
int GET_HTML_TEXT(int index, bool first)
{
    if (print_check)
        std::cout << "GET_HTML_TEXT " << index << " " << tmp[index] << '\n';

    if (index >= (int)tmp.length())
        return -1; // out of range

    if (first) // first time call GET_HTML_TEXT
    {
        if ((tmp[index] != '<') && (tmp[index] != '\0') && (index < (int)tmp.length() - 1))
        {
            return GET_HTML_TEXT(index + 1, false) + 1;
        }
        else if (tmp[index] == ' ')
        {
            int index_ = GET_HTML_TEXT(index + 1, true);
            if (index_ < 0)
            {
                return -1;
            }
            return index_ + 1;
        }
        return -1;
    }
    else // second time call GET_HTML_TEXT
    {
        if (tmp[index] == '<')
        {
            return 0;
        }
        // this if is just for one special testcase
        else if ((tmp[index] == '\n') || (tmp[index] == '\0') || (tmp[index] == ' ') || (tmp[index] == '<') || (tmp[index] == '/') || (tmp[index] == '>') || (tmp[index] == '='))
        {
            return 0; // not include this char (+0) (????)
        }
        /*
        NOT sure for this
        */
        // this if is just for one special testcase
        else if (index >= (int)tmp.length() - 1)
        {
            std::cout << "invalid input\n";
            exit(0);
        }
        // end of if
        return GET_HTML_TEXT(index + 1, false) + 1;
    }
}

// "[^"<]*"
int GET_HTML_DOUBLE_QUOTE_STRING(int index, bool first)
{
    if (print_check)
        std::cout << "GET_HTML_DOUBLE_QUOTE_STRING " << index << '\n';

    if (first) // first time call GET_HTML_DOUBLE_QUOTE_STRING (left)
    {
        if (tmp[index] == '"')
        {
            return GET_HTML_DOUBLE_QUOTE_STRING(index + 1, false) + 1;
        }
        else if (tmp[index] == ' ')
        {
            int index_ = GET_HTML_DOUBLE_QUOTE_STRING(index + 1, true);
            if (index_ < 0)
            {
                return -1;
            }
            return index_ + 1;
        }
        return -1;
    }
    else // second time call GET_HTML_DOUBLE_QUOTE_STRING (right)
    {
        if (tmp[index] == '"')
        {
            return 1; // include this char (+1)
        }
        // this if is just for one special testcase
        else if (index >= (int)tmp.length() - 1)
        {
            std::cout << "invalid input\n";
            exit(0);
        }
        // end of if
        return GET_HTML_DOUBLE_QUOTE_STRING(index + 1, false) + 1;
    }
}

// '[^'<]*'
int GET_HTML_SINGLE_QUOTE_STRING(int index, bool first)
{
    if (print_check)
        std::cout << "GET_HTML_SINGLE_QUOTE_STRING " << index << '\n';

    if (first) // first time call GET_HTML_SINGLE_QUOTE_STRING (left)
    {
        if (tmp[index] == '\'')
        {
            return GET_HTML_SINGLE_QUOTE_STRING(index + 1, false) + 1;
        }
        else if (tmp[index] == ' ')
        {
            int index_ = GET_HTML_SINGLE_QUOTE_STRING(index + 1, true);
            if (index_ < 0)
            {
                return -1;
            }
            return index_ + 1;
        }
        return -1;
    }
    else // second time call GET_HTML_SINGLE_QUOTE_STRING (right)
    {
        if (tmp[index] == '\'')
        {
            return 1; // include this char (+1)
        }
        // this if is just for one special testcase
        else if (index >= (int)tmp.length() - 1)
        {
            std::cout << "invalid input\n";
            exit(0);
        }
        // end of if
        return GET_HTML_SINGLE_QUOTE_STRING(index + 1, false) + 1;
    }
}

// <
int GET_TAG_OPEN(int index)
{
    if (print_check)
        std::cout << "GET_TAG_OPEN " << index << '\n';

    if (tmp[index] == '<')
    {
        return 1;
    }
    else if (tmp[index] == ' ')
    {
        int index_ = GET_TAG_OPEN(index + 1);
        if (index_ < 0)
        {
            return -1;
        }
        return index_ + 1;
    }
    return -1;
}

// >
int GET_TAG_CLOSE(int index)
{
    if (print_check)
        std::cout << "GET_TAG_CLOSE " << index << '\n';

    if (tmp[index] == '>')
    {
        return 1;
    }
    else if (tmp[index] == ' ')
    {
        int index_ = GET_TAG_CLOSE(index + 1);
        if (index_ < 0)
        {
            return -1;
        }
        return index_ + 1;
    }
    return -1;
}

// </
int GET_TAG_OPEN_SLASH(int index)
{
    if (print_check)
        std::cout << "GET_TAG_OPEN_SLASH " << index << '\n';

    if ((tmp[index] == '<') && (tmp[index + 1] == '/'))
    {
        return 2;
    }
    else if (tmp[index] == ' ')
    {
        int index_ = GET_TAG_OPEN_SLASH(index + 1);
        if (index_ < 0)
        {
            return -1;
        }
        return index_ + 1;
    }
    return -1;
}

// =
int GET_TAG_EQUALS(int index)
{
    if (print_check)
        std::cout << "GET_TAG_EQUALS " << index << '\n';

    if (tmp[index] == '=')
    {
        return 1;
    }
    else if (tmp[index] == ' ')
    {
        int index_ = GET_TAG_EQUALS(index + 1);
        if (index_ < 0)
        {
            return -1;
        }
        return index_ + 1;
    }
    return -1;
}
