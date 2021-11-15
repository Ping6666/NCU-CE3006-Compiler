#include <algorithm>
#include <iostream>
#include <vector>

struct FirstSet
{
    char Terminal_;
    std::string FirstSet_;

    FirstSet(char a, char b)
    {
        this->Terminal_ = a;
        this->FirstSet_ = b;
    }

    FirstSet(char a, std::string b)
    {
        this->Terminal_ = a;
        this->FirstSet_ = b;
    }
};

struct Index
{
    int pre;
    int post;

    Index(int a, int b)
    {
        this->pre = a;
        this->post = b;
    }
};

/*
global variable
*/

char tmp[1000];
std::vector<FirstSet> FirstSet_vector;
std::vector<Index> Index_vector;

/*
declaration FirstSet function
*/

void vectorAddItem(FirstSet firstset);
void vectorRemoveItem(char a, char b);
void vectorManipulate();
void vectorManipulate();

/*
public function for FirstSet vector
*/

void vectorAddItem(FirstSet firstset)
{

    if ((int)FirstSet_vector.size() == 0)
    {
        // empty vector
        FirstSet_vector.push_back(firstset);
        return;
    }

    for (int i = 0; i < (int)FirstSet_vector.size(); i++)
    {
        if (FirstSet_vector[i].Terminal_ == firstset.Terminal_)
        {
            // if match item
            FirstSet_vector[i].FirstSet_ = FirstSet_vector[i].FirstSet_ + firstset.FirstSet_;
            return;
        }
        else if (FirstSet_vector[i].Terminal_ - firstset.Terminal_ > 0)
        {
            /*
            this part just for the ans sort
            */

            // if no matching item
            FirstSet_vector.insert(FirstSet_vector.begin() + i, firstset);
            return;
        }
    }

    FirstSet_vector.push_back(firstset);

    return;
}

void vectorRemoveItem(char a, char b)
{
    // vectorManipulate();
    if ((int)FirstSet_vector.size() == 0)
    {
        return;
    }

    for (int i = 0; i < (int)FirstSet_vector.size(); i++)
    {
        if (FirstSet_vector[i].Terminal_ == a)
        {
            // if match item
            FirstSet_vector[i].FirstSet_.erase(std::remove(FirstSet_vector[i].FirstSet_.begin(), FirstSet_vector[i].FirstSet_.end(), b), FirstSet_vector[i].FirstSet_.end());
            return;
        }
        else if (FirstSet_vector[i].Terminal_ - a > 0)
        {
            return;
        }
    }

    return;
}

void vectorManipulate()
{
    for (int i = 0; i < (int)FirstSet_vector.size(); i++)
    {
        // manipulate the FirstSet_vector: sort and remove duplicate
        std::sort(FirstSet_vector[i].FirstSet_.begin(), FirstSet_vector[i].FirstSet_.end());
        FirstSet_vector[i].FirstSet_.erase(std::unique(FirstSet_vector[i].FirstSet_.begin(), FirstSet_vector[i].FirstSet_.end()), FirstSet_vector[i].FirstSet_.end());
    }
    return;
}

void vectorPrint()
{
    for (int i = 0; i < (int)FirstSet_vector.size(); i++)
    {
        std::cout << FirstSet_vector[i].Terminal_ << " ";
        std::cout << FirstSet_vector[i].FirstSet_ << "\n";
    }
    return;
}

/*
declaration function
*/

void parsernonTerminal(int preindex, int index);
void parserTerminal(int preindex, int index);

/*
main function
*/

int main()
{
    int index = 0;
    while (true)
    {
        char ch = getchar();
        if (ch == EOF)
        {
            break;
        }
        else
        {
            tmp[index] = ch;
            index++;
        }
    }

    // first set: only see self
    int preindex = 0;
    for (int i = 0; i < index; i++)
    {
        if (i == index - 1)
        {
            break;
        }
        else if (tmp[i] == '\n')
        {
            parsernonTerminal(preindex, i);
            Index_vector.push_back(Index(preindex, i));
            preindex = i + 1;
        }
    }

    // this may have BUG, may have diff compare to recursive
    for (int i = (int)Index_vector.size() - 1; i >= 0; i--)
    {
        parserTerminal(Index_vector[i].pre, Index_vector[i].post);
    }

    vectorManipulate();
    vectorPrint();
    std::cout << "END_OF_FIRST\n";
}

/*
parser function
*/

void parsernonTerminal(int preindex, int index)
{
    bool checker = true; // this means this is first nonTerminal.
    for (int i = preindex + 2; i < index; i++)
    {
        if (checker)
        {
            checker = false;
            if (!(tmp[i] >= 'A' && tmp[i] <= 'Z'))
            {
                vectorAddItem(FirstSet(tmp[preindex], tmp[i]));
            }
        }
        else if (!checker && tmp[i] == '|')
        {
            checker = true;
        }
    }
    return;
}

void parserTerminal(int preindex, int index)
{
    bool checker = true; // this means previous Terminal set can be an empty set.
    for (int i = preindex + 2; i < index; i++)
    {
        if (checker)
        {
            if (tmp[i] >= 'A' && tmp[i] <= 'Z')
            {
                vectorManipulate();
                bool emptyset = false;
                for (int j = 0; j < (int)FirstSet_vector.size(); j++)
                {
                    if (emptyset)
                    {
                        break;
                    }
                    if (FirstSet_vector[j].Terminal_ == tmp[i])
                    {
                        bool shouldHaveEmptySet = false;
                        for (int k = 0; k < (int)FirstSet_vector[j].FirstSet_.size(); k++)
                        {
                            if (emptyset)
                            {
                                break;
                            }
                            if (FirstSet_vector[j].FirstSet_[k] == ';')
                            {
                                // first set contain empty set (;)
                                if (i == index - 1 || tmp[i + 1] == '|')
                                {
                                    shouldHaveEmptySet = true;
                                }
                                checker = true;
                                emptyset = true;
                            }
                        }
                        // first set did not contain empty set (;)
                        if (emptyset)
                        {
                            if (shouldHaveEmptySet)
                            {
                                vectorAddItem(FirstSet(tmp[preindex], FirstSet_vector[j].FirstSet_));
                            }
                            else
                            {
                                vectorRemoveItem(tmp[i], ';');
                                vectorAddItem(FirstSet(tmp[preindex], FirstSet_vector[j].FirstSet_));
                                /*
                                After calling vectorAddItem index j maybe change,
                                therefore use tmp[i] to remove and add item.
                                */
                                vectorAddItem(FirstSet(tmp[i], ';'));
                            }
                        }
                        else
                        {
                            vectorAddItem(FirstSet(tmp[preindex], FirstSet_vector[j].FirstSet_));
                            checker = false;
                            break;
                        }
                    }
                }
                // this can be no found (maybe wrong)
            }
            else if (tmp[i] != '|' && !(tmp[i] >= 'A' && tmp[i] <= 'Z'))
            {
                vectorAddItem(FirstSet(tmp[preindex], tmp[i]));
                checker = false;
            }
        }
        else if (!checker && tmp[i] == '|')
        {
            checker = true;
        }
    }
    return;
}
