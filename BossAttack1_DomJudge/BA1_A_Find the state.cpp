#include <iostream>

char tmp[25];
int index = 0;

// -1 is no, other is same
int FSM(int id_now, int state)
{
    if (state <= 0)
    {
        return -1;
    }

    if (tmp[id_now] == '$')
    {
        return state;
    }
    else if (id_now >= index)
    {
        return -1;
    }

    if (state == 1)
    {
        if (tmp[id_now] == 'a')
        {
            return FSM(id_now + 1, 2);
        }
        else if (tmp[id_now] == 'b')
        {
            return FSM(id_now + 1, 3);
        }
        else
        {
            return -1;
        }
    }
    else if (state == 2)
    {
        if (tmp[id_now] == 'a')
        {
            return FSM(id_now + 1, 2);
        }
        else if (tmp[id_now] == 'b')
        {
            return FSM(id_now + 1, 4);
        }
        else
        {
            return -1;
        }
    }
    else if (state == 3)
    {
        if (tmp[id_now] == 'a')
        {
            return FSM(id_now + 1, 3);
        }
        else if (tmp[id_now] == 'c')
        {
            return FSM(id_now + 1, 4);
        }
        else
        {
            return -1;
        }
    }
    else if (state == 4)
    {
        if (tmp[id_now] != '$')
        {
            return -1;
        }
        else
        {
            return state;
        }
    }
    else
    {
        return -1;
    }
}

int main()
{
    while (true)
    {
        char ch = getchar();
        if (ch == EOF)
        {
            break;
        }
        else if (ch == '\n')
        {
            break;
        }
        else
        {
            tmp[index] = ch;
            index++;
        }
    }

    int result_ = FSM(0, 1);

    if (result_ <= 2) // below then 2 is not end state
    {
        std::cout << "NO";
    }
    else
    {
        std::cout << "YES s" << result_;
    }

    return 0;
}