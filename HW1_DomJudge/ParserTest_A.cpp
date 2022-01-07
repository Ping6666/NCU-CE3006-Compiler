// #include <algorithm>
#include <iostream>
#include <vector>

/*
Recursive-Decent-Parsing:
Since need to fit the question needed, therefore try to use many function and if / else.
*/

struct AnsForm
{
    std::string TerminalName;
    std::string ExpressionContent;

    AnsForm(std::string a, std::string b)
    {
        this->TerminalName = a;
        this->ExpressionContent = b;
    }

    AnsForm(std::string a, char b)
    {
        this->TerminalName = a;
        this->ExpressionContent = b;
    }
};

// global variable
std::string tmp;
std::vector<AnsForm> ansForm;

void printVector()
{
    for (int i = 0; i < (int)ansForm.size(); i++)
    {
        if (ansForm[i].TerminalName != "STRLIT")
        {
            std::cout << ansForm[i].TerminalName << " " << ansForm[i].ExpressionContent << "\n";
        }
        else
        {
            std::cout << ansForm[i].TerminalName << " \"" << ansForm[i].ExpressionContent << "\"\n";
        }
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
int GETstmts(int index);
int GETstmt(int index);
int GETprimary(int index);
int GETprimary_tail(int index);

// Terminal: Regular Expression
int GET_ID(int index, bool first = true);     // [A-Z a-z _][A-Z a-z 0-9 _]*
int GET_STRLIT(int index, bool first = true); // "[^"]*"
int GET_LBR(int index);                       // (
int GET_RBR(int index);                       // )
int GET_DOT(int index);                       // .

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
            tmp += " ";
        }
        else
        {
            tmp += ch;
        }
    }
    stringManipulate();

    if (GETstmts(0) < 0)
    {
        std::cout << "invalid input\n";
        return 0;
    }

    printVector();
    return 0;
}

/*
Productions
*/

int GETstmts(int index)
{
    int index_ = GETstmt(index);
    index += index_; // this shoud not do first, but it is ok to do first here
    if (index_ < 0)
    {
        return -1;
    }
    else if (index >= (int)tmp.length() - 1)
    {
        return 0; // success
    }
    else if (index_ == 0)
    {
        // std::cout << "something went wrong.\n";
        /*
        Note:
        if it is valid expression, it should never went into this state.
        if it went to this state which means that the expression must be a invalid expression.
        */
        return -2;
    }
    return GETstmts(index);
}

int GETstmt(int index)
{
    int index_1 = GETprimary(index);
    if (index_1 < 0)
    {
        int index_2 = GET_STRLIT(index);
        if (index_2 < 0)
        {
            return 0; // this is λ (0)
        }
        return index_2;
    }
    else
    {
        return index_1;
    }
    // return index;
}

int GETprimary(int index)
{
    int index_1 = GET_ID(index);
    if (index_1 < 0)
    {
        return -1; // this is diff. (maybe is 0)
    }
    index += index_1;
    int index_2 = GETprimary_tail(index);
    if (index_2 < 0)
    {
        return -1;
    }
    index += index_2;
    return index_1 + index_2;
}

int GETprimary_tail(int index)
{
    int index_1 = GET_DOT(index);
    if (index_1 < 0)
    {
        int index_1_1 = GET_LBR(index);
        if (index_1_1 < 0)
        {
            return 0; // this is diff. (maybe is 0)
        }
        index += index_1_1;
        int index_1_2 = GETstmt(index);
        if (index_1_2 < 0)
        {
            return -1;
        }
        index += index_1_2;
        int index_1_3 = GET_RBR(index);
        if (index_1_3 < 0)
        {
            return -1;
        }
        index += index_1_3;
        int index_1_4 = GETprimary_tail(index);
        if (index_1_4 < 0)
        {
            return -1;
        }
        index += index_1_4;
        return index_1_1 + index_1_2 + index_1_3 + index_1_4;
    }
    else
    {
        index += index_1;
        int index_2_1 = GET_ID(index);
        if (index_2_1 < 0)
        {
            return -1; // this is diff. (maybe is 0)
        }
        index += index_2_1;
        int index_2_2 = GETprimary_tail(index);
        if (index_2_2 < 0)
        {
            return -1;
        }
        index += index_2_2;
        return index_1 + index_2_1 + index_2_2;
    }
    return 0; // this is λ (0)
}

/*
Terminal: Regular Expression
*/

int GET_ID(int index, bool first)
{
    if (first) // first time call GET_ID (smaller rule)
    {
        if (((tmp[index] >= 'a') && (tmp[index] <= 'z')) || ((tmp[index] >= 'A') && (tmp[index] <= 'Z')) || (tmp[index] == '_'))
        {
            ansForm.push_back(AnsForm("ID", tmp[index]));
            int index_ = GET_ID(index + 1, false);
            if (index_ < 0)
            {
                return -1;
            }
            return index_ + 1;
        }
        else if (tmp[index] == ' ')
        {
            int index_ = GET_ID(index + 1, true);
            if (index_ < 0)
            {
                return -1;
            }
            return index_ + 1;
        }
        return -1;
    }
    else // second time call GET_ID (bigger rule)
    {
        if (((tmp[index] >= 'a') && (tmp[index] <= 'z')) || ((tmp[index] >= 'A') && (tmp[index] <= 'Z')) || (tmp[index] == '_') || ((tmp[index] >= '0') && (tmp[index] <= '9')))
        {
            AnsForm tmpForm = ansForm.back();
            ansForm.pop_back();
            ansForm.push_back(AnsForm("ID", tmpForm.ExpressionContent + tmp[index]));
            int index_ = GET_ID(index + 1, false);
            if (index_ < 0)
            {
                return -1;
            }
            return index_ + 1;
        }
        else if ((tmp[index] == ' ') || (tmp[index] == '.') || (tmp[index] == '(') || (tmp[index] == ')') || (tmp[index] == '"'))
        {
            return 0; // not include this char (+0) (????)
        }
        ansForm.pop_back();
        return -1;
    }
}

int GET_STRLIT(int index, bool first)
{
    if (first) // first time call GET_STRLIT (left)
    {
        if (tmp[index] == '"')
        {
            ansForm.push_back(AnsForm("STRLIT", ""));
            return GET_STRLIT(index + 1, false) + 1;
        }
        else if (tmp[index] == ' ')
        {
            int index_ = GET_STRLIT(index + 1, true);
            if (index_ < 0)
            {
                return -1;
            }
            return index_ + 1;
        }
        return -1;
    }
    else // second time call GET_STRLIT (right)
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
        AnsForm tmpForm = ansForm.back();
        ansForm.pop_back();
        ansForm.push_back(AnsForm("STRLIT", tmpForm.ExpressionContent + tmp[index]));
        return GET_STRLIT(index + 1, false) + 1;
    }
}

int GET_LBR(int index)
{
    if (tmp[index] == '(')
    {
        ansForm.push_back(AnsForm("LBR", "("));
        return 1;
    }
    else if (tmp[index] == ' ')
    {
        int index_ = GET_LBR(index + 1);
        if (index_ < 0)
        {
            return -1;
        }
        return index_ + 1;
    }
    return -1;
}

int GET_RBR(int index)
{
    if (tmp[index] == ')')
    {
        ansForm.push_back(AnsForm("RBR", ")"));
        return 1;
    }
    else if (tmp[index] == ' ')
    {
        int index_ = GET_RBR(index + 1);
        if (index_ < 0)
        {
            return -1;
        }
        return index_ + 1;
    }
    return -1;
}

int GET_DOT(int index)
{
    if (tmp[index] == '.')
    {
        ansForm.push_back(AnsForm("DOT", "."));
        return 1;
    }
    else if (tmp[index] == ' ')
    {
        int index_ = GET_DOT(index + 1);
        if (index_ < 0)
        {
            return -1;
        }
        return index_ + 1;
    }
    return -1;
}
