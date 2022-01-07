#include <algorithm>
#include <iostream>
#include <vector>

/*
FIRST
*/
// To compute FIRST(X) for grammar symbol X, apply the following rules until no more terminals or λ can be added to it.
// If X is a terminal , then FIRST(X) = {X}
// If X is a non-terminal and X → Y1, Y2, …, Yk is a production for some k>=1,
// then place “a” in FIRST(X) if for some i, “a” is in FIRST(Yi), and λ is in all of FIRST(Y1),…,FIRST(Yi-1).
// If λ is in FIRST(Yj) for all j=1,2,…,k, then add λ to FIRST(X).
// If X → λ is a production, then add λ to FIRST(X).

/*
FOLLOW
*/
// To compute FOLLOW(B) for non-terminal B:
// Place $ in FOLLOW(S), where S is the start symbol, and $ is the input right end-marker.
// if there is a production A → α B β, then everything in FIRST(β) except λ is in FOLLOW(B).
// (a) if there is a production A → α B,
// (b) or A → α B β, where FIRST(β) contains λ, then everything in FOLLOW(A) is in FOLLOW(B).

struct GrammarSet // FirstSet, FollowSet
{
    char Terminal_;
    std::string Set_;

    GrammarSet(char a, char b)
    {
        this->Terminal_ = a;
        this->Set_ = b;
    }

    GrammarSet(char a, std::string b)
    {
        this->Terminal_ = a;
        this->Set_ = b;
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
std::vector<GrammarSet> FirstSet_vector;
std::vector<GrammarSet> FollowSet_vector;
std::vector<Index> Index_vector;

/*
declaration GrammarSet function
*/

// select: 0, first set; 1, follow set.
void vectorAddItem(GrammarSet grammarSet, int select);
void vectorRemoveItem(char a, char b, int select);
void vectorPrint(int select);
void vectorManipulate();

/*
public function for GrammarSet vector
*/

void vectorAddItem(GrammarSet grammarSet, int select)
{
    std::vector<GrammarSet> *ptr;
    if (select == 0) // first set vector
    {
        ptr = (std::vector<GrammarSet> *)&FirstSet_vector;
    }
    else // follow set vector
    {
        ptr = (std::vector<GrammarSet> *)&FollowSet_vector;
    }

    if ((int)(*ptr).size() == 0)
    {
        // empty vector
        (*ptr).push_back(grammarSet);
        return;
    }

    for (int i = 0; i < (int)(*ptr).size(); i++)
    {
        if ((*ptr)[i].Terminal_ == grammarSet.Terminal_)
        {
            // if match item
            (*ptr)[i].Set_ = (*ptr)[i].Set_ + grammarSet.Set_;
            return;
        }
        else if ((*ptr)[i].Terminal_ - grammarSet.Terminal_ > 0)
        {
            /*
            this part just for the ans sort
            */
            // if no matching item
            (*ptr).insert((*ptr).begin() + i, grammarSet);
            return;
        }
    }

    (*ptr).push_back(grammarSet);
    return;
}

void vectorRemoveItem(char a, char b, int select)
{
    vectorManipulate();
    std::vector<GrammarSet> *ptr;
    if (select == 0) // first set vector
    {
        ptr = (std::vector<GrammarSet> *)&FirstSet_vector;
    }
    else // follow set vector
    {
        ptr = (std::vector<GrammarSet> *)&FollowSet_vector;
    }
    if ((int)(*ptr).size() == 0)
    {
        return;
    }

    for (int i = 0; i < (int)(*ptr).size(); i++)
    {
        if ((*ptr)[i].Terminal_ == a)
        {
            // if match item
            (*ptr)[i].Set_.erase(std::remove((*ptr)[i].Set_.begin(), (*ptr)[i].Set_.end(), b), (*ptr)[i].Set_.end());
            return;
        }
        else if ((*ptr)[i].Terminal_ - a > 0)
        {
            return;
        }
    }

    return;
}

void vectorManipulate()
{
    // manipulate both vector
    std::vector<GrammarSet> *ptr;
    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
            ptr = (std::vector<GrammarSet> *)&FirstSet_vector;
        }
        else
        {
            ptr = (std::vector<GrammarSet> *)&FollowSet_vector;
        }
        for (int i = 0; i < (int)(*ptr).size(); i++)
        {
            // manipulate the FirstSet_vector, FollowSet_vector: sort and remove duplicate
            std::sort((*ptr)[i].Set_.begin(), (*ptr)[i].Set_.end());
            (*ptr)[i].Set_.erase(std::unique((*ptr)[i].Set_.begin(), (*ptr)[i].Set_.end()), (*ptr)[i].Set_.end());
        }
    }

    return;
}

void vectorPrint(int select)
{
    std::vector<GrammarSet> *ptr;
    if (select == 0) // first set vector
    {
        ptr = (std::vector<GrammarSet> *)&FirstSet_vector;
    }
    else // follow set vector
    {
        ptr = (std::vector<GrammarSet> *)&FollowSet_vector;
    }

    for (int i = 0; i < (int)(*ptr).size(); i++)
    {
        std::cout << (*ptr)[i].Terminal_ << " ";
        std::cout << (*ptr)[i].Set_ << "\n";
    }
    return;
}

/*
declaration function
*/

void parsernonTerminal(int preindex, int index);
void parserTerminal(int preindex, int index);
void followInFirst(int preindex, int index);
void followNonFirst(int preindex, int index);

/*
main function
*/

int main()
{
    int index = 0;
    int getChar = 1;
    int checkIndex = 0;
    std::string tmpEND = "END_OF_GRAMMAR";
    int tmpEND_len = tmpEND.size();

    while (true)
    {
        char ch = getchar();
        if (ch == EOF)
        {
            break;
        }
        else if (getChar == 1)
        {
            tmp[index] = ch;
            index++;
            if (ch == tmpEND[checkIndex])
            {
                checkIndex += 1;
            }
            else
            {
                checkIndex = 0;
            }
            if (checkIndex == tmpEND_len)
            {
                getChar = 0;
            }
            else if (ch == '\n')
            {
                checkIndex = 0;
            }
        }
    }

    // first set: only see self
    int preindex = 0;
    int isFirst = 0;
    for (int i = 0; i < index; i++)
    {
        if (i == index - 1)
        {
            break;
        }
        else if (tmp[i] == '\n' && isFirst == 0)
        {
            parsernonTerminal(preindex, i);

            Index_vector.push_back(Index(preindex, i));
            preindex = i + 1;
        }
    }

    // first set: this may have BUG, may have diff compare to recursive
    for (int i = (int)Index_vector.size() - 1; i >= 0; i--)
    {
        parserTerminal(Index_vector[i].pre, Index_vector[i].post);
    }

    vectorManipulate();

    // follow $
    vectorAddItem(GrammarSet(tmp[0], (char)'$'), 1);

    // followInFirst
    for (int i = (int)Index_vector.size() - 1; i >= 0; i--)
    {
        followInFirst(Index_vector[i].pre, Index_vector[i].post);
    }

    // followNonFirst
    for (int indexNow = 0; indexNow < (int)Index_vector.size(); indexNow++)
    {
        for (int i = (int)Index_vector.size() - 1; i >= 0; i--)
        {
            followNonFirst(Index_vector[i].pre, Index_vector[i].post);
        }
    }

    vectorManipulate();

    vectorPrint(0);
    std::cout << "END_OF_FIRST\n";

    vectorPrint(1);
    std::cout << "END_OF_FOLLOW\n";
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
                vectorAddItem(GrammarSet(tmp[preindex], tmp[i]), 0);
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
                        for (int k = 0; k < (int)FirstSet_vector[j].Set_.size(); k++)
                        {
                            if (emptyset)
                            {
                                break;
                            }
                            if (FirstSet_vector[j].Set_[k] == ';')
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
                                vectorAddItem(GrammarSet(tmp[preindex], FirstSet_vector[j].Set_), 0);
                            }
                            else
                            {
                                vectorRemoveItem(tmp[i], ';', 0);
                                vectorAddItem(GrammarSet(tmp[preindex], FirstSet_vector[j].Set_), 0);
                                /*
                                After calling vectorAddItem index j maybe change,
                                therefore use tmp[i] to remove and add item.
                                */
                                vectorAddItem(GrammarSet(tmp[i], ';'), 0);
                            }
                        }
                        else
                        {
                            vectorAddItem(GrammarSet(tmp[preindex], FirstSet_vector[j].Set_), 0);
                            checker = false;
                            break;
                        }
                    }
                }
                // this can be no found (maybe wrong)
            }
            else if (tmp[i] != '|' && !(tmp[i] >= 'A' && tmp[i] <= 'Z'))
            {
                vectorAddItem(GrammarSet(tmp[preindex], tmp[i]), 0);
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

void followInFirst(int preindex, int index)
{
    bool checker; // this means this is first nonTerminal.
    for (int i = preindex + 2; i < index - 1; i++)
    {
        checker = true;
        if (tmp[i] >= 'A' && tmp[i] <= 'Z')
        {
            for (int j = i + 1; j < index; j++)
            {
                vectorManipulate();
                if (!checker)
                {
                    break;
                }
                if (tmp[j] >= 'A' && tmp[j] <= 'Z')
                {
                    for (int k = 0; k < (int)FirstSet_vector.size(); k++)
                    {
                        if (FirstSet_vector[k].Terminal_ == tmp[j])
                        {
                            bool emptyset = false;
                            for (int l = 0; l < (int)FirstSet_vector[k].Set_.size(); l++)
                            {
                                if (FirstSet_vector[k].Set_[l] == ';')
                                {
                                    emptyset = true;
                                    break; // l loop
                                }
                            }
                            if (emptyset)
                            {
                                vectorRemoveItem(tmp[j], ';', 0);
                                vectorAddItem(GrammarSet(tmp[i], FirstSet_vector[k].Set_), 1);
                                vectorAddItem(GrammarSet(tmp[j], ';'), 0);
                                // checker = true;
                                break; // k loop
                            }
                            else
                            {
                                vectorAddItem(GrammarSet(tmp[i], FirstSet_vector[k].Set_), 1);
                                checker = false;
                                break; // k loop
                            }
                        }
                    }
                }
                else
                {
                    checker = false;
                    if (tmp[j] != '|')
                    {
                        vectorAddItem(GrammarSet(tmp[i], tmp[j]), 1);
                    }
                }
            }
        }
    }
    return;
}

void followNonFirst(int preindex, int index)
{
    bool checker; // this means this is first nonTerminal.
    for (int i = index - 1; i >= preindex + 2; i--)
    {
        if (!(tmp[i] >= 'A' && tmp[i] <= 'Z'))
        {
            continue;
        }

        checker = true;
        for (int j = i + 1; j < index; j++)
        {
            vectorManipulate();
            if (!checker)
            {
                break;
            }
            if (tmp[j] >= 'A' && tmp[j] <= 'Z')
            {
                for (int k = 0; k < (int)FirstSet_vector.size(); k++)
                {
                    if (FirstSet_vector[k].Terminal_ == tmp[j])
                    {
                        bool emptyset = false;
                        for (int l = 0; l < (int)FirstSet_vector[k].Set_.size(); l++)
                        {
                            if (FirstSet_vector[k].Set_[l] == ';')
                            {
                                emptyset = true;
                                break; // l loop
                            }
                        }
                        if (emptyset)
                        {
                            checker = true;
                            break; // k loop
                        }
                        else
                        {
                            checker = false;
                            break; // k loop
                        }
                    }
                }
            }
            else
            {
                checker = false;
                if (tmp[j] == '|')
                {
                    checker = true;
                }
                break;
            }
        }

        vectorManipulate();

        // if every terminal after index = i 's first set contain empty set
        if (checker)
        {
            for (int j = 0; j < (int)FollowSet_vector.size(); j++)
            {
                if (FollowSet_vector[j].Terminal_ == tmp[preindex])
                {
                    // follow add to follow
                    vectorAddItem(GrammarSet(tmp[i], FollowSet_vector[j].Set_), 1);
                    break;
                }
            }
        }
    }
    return;
}
