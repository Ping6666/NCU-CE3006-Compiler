#include <iostream>

// all test case will be legal

std::string inputString;

void stringManipulate()
{
    bool set = false;
    for (int i = 0; i < (int)inputString.length(); i++)
    {
        if (set)
        {
            if (inputString[i] == ' ')
            {
                inputString.erase(inputString.begin() + i);
                i--;
            }
            else
            {
                set = false;
            }
        }
        else
        {
            if (inputString[i] == ' ')
            {
                set = true;
            }
        }
    }
    return;
}

int main()
{
    inputString.clear();
    while (true)
    {
        char ch = getchar();
        if (ch == EOF)
        {
            break;
        }
        else if (ch == '\n')
        {
            inputString += " ";
        }
        else
        {
            inputString += ch;
        }
    }
    stringManipulate();

    bool inTagDef = false;
    int id_now = 0;
    bool firstPrint = false;
    bool QUOTE_STRING = false;
    while (true)
    {
        if (id_now >= inputString.length())
        {
            break;
        }

        if ((id_now + 1 < inputString.length()) && (inputString[id_now] == '<') && (inputString[id_now + 1] == '/'))
        {
            inTagDef = true;
            if (firstPrint)
            {
                std::cout << "\n";
                firstPrint = false;
            }
            std::cout << "TAG_OPEN_SLASH </\n";
            id_now += 1;
        }
        else if (inputString[id_now] == '<')
        {
            inTagDef = true;
            if (firstPrint)
            {
                std::cout << "\n";
                firstPrint = false;
            }
            std::cout << "TAG_OPEN <\n";
        }
        else if (inputString[id_now] == '>')
        {
            inTagDef = false;
            if (firstPrint)
            {
                std::cout << "\n";
                firstPrint = false;
            }
            std::cout << "TAG_CLOSE >\n";
        }
        else if (inputString[id_now] == '=')
        {
            if (firstPrint)
            {
                std::cout << "\n";
                firstPrint = false;
            }
            std::cout << "TAG_EQUALS =\n";
        }
        else if (inputString[id_now] == '\'' && inTagDef == true) // 1
        {
            inTagDef = false;
            QUOTE_STRING = true;
            if (firstPrint)
            {
                std::cout << "\n";
                firstPrint = false;
            }
            // firstPrint = false;
            std::cout << "SINGLE_QUOTE_STRING ";
        }
        else if (inputString[id_now] == '\'' && inTagDef == false && QUOTE_STRING == true) // 2
        {
            inTagDef = true;
            QUOTE_STRING = false;
        }
        else if (inputString[id_now] == '"' && inTagDef == true) // 1
        {
            inTagDef = false;
            QUOTE_STRING = true;
            if (firstPrint)
            {
                std::cout << "\n";
                firstPrint = false;
            }
            // firstPrint = false;
            std::cout << "DOUBLE_QUOTE_STRING ";
        }
        else if (inputString[id_now] == '"' && inTagDef == false && QUOTE_STRING == true) // 2
        {
            inTagDef = true;
            QUOTE_STRING = false;
        }
        else if (inputString[id_now] == ' ')
        {
            // continue;
            if (inTagDef)
            {
                std::cout << "\n";
                // inTagDef = false;
                firstPrint = false;
            }
            else if (firstPrint)
            {
                std::cout << " ";
            }
        }
        else if (inTagDef) // generate TAG_NAME token
        {
            if (!firstPrint)
            {
                firstPrint = true;
                std::cout << "TAG_NAME ";
            }
            std::cout << inputString[id_now];
        }
        else // generate HTML_TEXT token
        {
            if ((QUOTE_STRING) && (inputString[id_now] == '\'' || inputString[id_now] == '"'))
            {
                QUOTE_STRING = false;
            }
            else
            {
                if (QUOTE_STRING)
                {
                    firstPrint = true;
                }
                if ((!firstPrint) && (!QUOTE_STRING))
                {
                    firstPrint = true;
                    std::cout << "HTML_TEXT ";
                }
                std::cout << inputString[id_now];
            }
        }
        id_now += 1;
    }

    return 0;
}
