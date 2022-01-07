#include "ast.h"
#define maplength 3

/* checker is for the error detection. */

// index: 0 is for the global var. map
// index: 1 is for the named fun. map        (can be found by fun. name)
// index: 2 is for the named fun.'s var. map (can be found by fun. name)
std::map<std::string, ASTnode *> var_map[maplength];
std::stack<ASTnode *> idparamnodes; // local ids (contain id-param pairs) under a called function

bool insertmap(int index, std::string insertname, ASTnode *insertnode)
{
    if (index >= maplength || index < 0)
        return false;
    ASTnode *tmpnode = duplicatenodes(insertnode);
    var_map[index].insert(std::pair<std::string, ASTnode *>(insertname, tmpnode));
    return true;
}

bool findmap(int index, std::string findname, bool checker, ASTnode **findnode)
{
    if (index >= maplength || index < 0)
        return false;
    std::map<std::string, ASTnode *>::iterator mapiter;
    mapiter = var_map[index].find(findname);
    if (mapiter != var_map[index].end())
    {
        *findnode = duplicatenodes(mapiter->second);
        return true;
    }
    /* checker */
    if (checker)
        printf("Find error! Check find str or find field.\n");
    return false;
}

void freemap()
{
    /* suspend this function because it poison the tree */
    return;
    // free map
    for (int i = 0; i < maplength; i++)
    {
        std::map<std::string, ASTnode *>::iterator mapiter;
        for (const auto &mapiter : var_map[i])
        {
            freenodes(mapiter.second);
        }
        var_map[i].clear();
    }
    return;
}

ASTnode *manipulatenodes(ASTnode *idnodes, ASTnode *paramnodes)
{
    /* function for pairing ids' name and params */
    if (idnodes == NULL)
        return NULL;
    if ((idnodes->left != NULL) && (paramnodes->left != NULL))
    {
        idnodes->left = manipulatenodes(idnodes->left, paramnodes->left);
    }
    if ((idnodes->right != NULL) && (paramnodes->right != NULL))
    {
        idnodes->right = manipulatenodes(idnodes->right, paramnodes->right);
    }
    if (idnodes->node_type == ast_id)
    {
        ((ASTnode_id *)idnodes)->target = paramnodes;
    }
    return idnodes;
}

ASTnode *duplicatenodes(ASTnode *rootnode)
{
    if (rootnode == NULL)
        return NULL;
    ASTnode *newnode = NULL;
    if (rootnode->node_type == ast_bool)
    {
        newnode = mallocnode(rootnode->node_type, ((ASTnode_bool *)rootnode)->value);
    }
    else if (rootnode->node_type == ast_num)
    {
        newnode = mallocnode(rootnode->node_type, ((ASTnode_num *)rootnode)->value);
    }
    else if (rootnode->node_type == ast_id)
    {
        newnode = mallocnode(rootnode->node_type, ((ASTnode_id *)rootnode)->name);
        ((ASTnode_id *)newnode)->target = ((ASTnode_id *)rootnode)->target;
        // ((ASTnode_id *)newnode)->target = duplicatenodes(((ASTnode_id *)rootnode)->target);
    }
    else if (rootnode->node_type == ast_if)
    {
        newnode = (ASTnode *)malloc(sizeof(ASTnode_if));
        newnode->node_type = ast_if;
        ((ASTnode_if *)newnode)->statement = duplicatenodes(((ASTnode_if *)rootnode)->statement);
    }
    else
    {
        newnode = (ASTnode *)malloc(sizeof(ASTnode));
        newnode->node_type = rootnode->node_type;
    }
    newnode->left = duplicatenodes(rootnode->left);
    newnode->right = duplicatenodes(rootnode->right);
    /* checker */
    if (newnode == NULL)
    {
        printf("Duplicate nodes error!\n");
    }
    return newnode;
}

ASTnode *mallocaddnode(ASTtype newtype, ASTnode *newleft, ASTnode *newright)
{
    ASTnode *newnode;
    newnode = (ASTnode *)malloc(sizeof(ASTnode));
    newnode->node_type = newtype;
    newnode->left = newleft;
    newnode->right = newright;
    return (ASTnode *)newnode;
}

ASTnode *mallocaddnode(ASTtype newtype, ASTnode *newleft, ASTnode *newright, ASTnode *newstatement)
{
    ASTnode_if *newnode = (ASTnode_if *)malloc(sizeof(ASTnode_if));
    newnode->node_type = newtype;
    newnode->left = newleft;
    newnode->right = newright;
    newnode->statement = newstatement;
    return (ASTnode *)newnode;
}

ASTnode *mallocnode(ASTtype newtype, bool newvalue)
{
    if (newtype == ast_bool)
    {
        ASTnode_bool *newnode = (ASTnode_bool *)malloc(sizeof(ASTnode_bool));
        newnode->node_type = newtype;
        newnode->value = newvalue;
        return (ASTnode *)newnode;
    }
    return NULL;
}

ASTnode *mallocnode(ASTtype newtype, int newvalue)
{
    if (newtype == ast_num)
    {
        ASTnode_num *newnode = (ASTnode_num *)malloc(sizeof(ASTnode_num));
        newnode->node_type = newtype;
        newnode->value = newvalue;
        return (ASTnode *)newnode;
    }
    return NULL;
}

ASTnode *mallocnode(ASTtype newtype, std::string newname)
{
    if (newtype == ast_id)
    {
        ASTnode_id *newnode = (ASTnode_id *)malloc(sizeof(ASTnode_id));
        newnode->node_type = newtype;
        /* may contain possible error (seg. fault), see c++ string cow rule. */
        // BUG under renewing a std::string (Bonus 3: exps under same fun_body)
        newnode->name = newname;
        // newnode->name.assign(newname, 0, newname.size());
        return (ASTnode *)newnode;
    }
    return NULL;
}

ASTnode *mallocnode(ASTtype newtype, bool newbvalue, int newivalue)
{
    if (newtype == ast_equal_result)
    {
        ASTnode_equal *newnode = (ASTnode_equal *)malloc(sizeof(ASTnode_equal));
        newnode->node_type = newtype;
        newnode->bvalue = newbvalue;
        newnode->ivalue = newivalue;
        return (ASTnode *)newnode;
    }
    return NULL;
}

void freenodes(ASTnode *nownode)
{
    /* tester */
    /* suspend this function because it poison the tree */
    return;
    if (nownode == NULL)
        return;
    if (nownode->node_type == ast_if)
    {
        freenodes(((ASTnode_if *)nownode)->statement);
    }
    else if (nownode->node_type == ast_id)
    {
        freenodes(((ASTnode_id *)nownode)->target);
        /* free std::string is not necessary */
        ((ASTnode_id *)nownode)->name.clear();         // clear contain, not equivalent with free
        ((ASTnode_id *)nownode)->name.shrink_to_fit(); // notify os the space can be reuse, also not equivalent with free
    }
    freenodes(nownode->left);
    freenodes(nownode->right);
    free(nownode);
    return;
}

/* do not free this node, it will cause idparamnodes break */
bool findidnode(ASTnode *nownode, std::string idname, ASTnode **findnode)
{
    /* this function has a big time complexity (BIG-O), about O(N) */
    // the input maybe need to change too may global var.
    if (nownode == NULL)
        return false;
    std::string checkstr = "";
    returnnode(nownode, ast_id, &checkstr);
    if (idname == checkstr && ((ASTnode_id *)nownode)->target != NULL)
    {
        *findnode = ((ASTnode_id *)nownode)->target;
        /* tester */
        // printf("match node name:%s.\n", checkstr.c_str());
        // printASTtype(*findnode);
        return true;
    }
    return (findidnode(nownode->left, idname, findnode) || findidnode(nownode->right, idname, findnode));
}

bool returnnode(ASTnode *nownode, ASTtype prevtype, std::string *returnstring)
{
    if (nownode == NULL)
        return false;
    if (nownode->node_type == prevtype)
    {
        if (prevtype == ast_id)
        {
            *returnstring = ((ASTnode_id *)nownode)->name;
            return true;
        }
    }
    if (nownode->node_type == ast_ids && prevtype == ast_id)
        return false;
    /* tester */
    // printf("(ast_id) ");
    /* checker */
    printf("Type error!\n");
    exit(0); /* dangerous move */
}

bool returnnode(ASTnode *nownode, ASTtype prevtype, bool *returnbool, int *returnnum)
{
    if (nownode == NULL)
        return false;
    if (nownode->node_type == prevtype)
    {
        if (prevtype == ast_bool)
        {
            *returnbool = ((ASTnode_bool *)nownode)->value;
            return true;
        }
        else if (prevtype == ast_num)
        {
            *returnnum = ((ASTnode_num *)nownode)->value;
            return true;
        }
        else if (prevtype == ast_equal_result)
        {
            *returnbool = ((ASTnode_equal *)nownode)->bvalue;
            *returnnum = ((ASTnode_equal *)nownode)->ivalue;
            return true;
        }
    }
    else if (nownode->node_type == ast_equal_result)
    {
        if (prevtype == ast_bool)
        {
            *returnbool = ((ASTnode_equal *)nownode)->bvalue;
            return true;
        }
    }
    else if (nownode->node_type == ast_id) // define var. or var. inside fun.
    {
        std::string tmpstr;
        returnnode(nownode, ast_id, &tmpstr);
        ASTnode *targetnode = NULL; /* store the target node with same name as tmpstr' */
        /* checker */
        if (idparamnodes.empty() && !findmap(0, tmpstr, true, &targetnode))
        {
            printf("ID:%s found, but no id list.\n", tmpstr.c_str());
            exit(0);
        }
        if ((!idparamnodes.empty() && findidnode(idparamnodes.top(), tmpstr, &targetnode)) || findmap(0, tmpstr, true, &targetnode))
        {
            /* 1st condition var. inside fun. */
            // The local var.s in calling fun. can be found in global var. idnodes.
            // And, it and it's value was been pair by function manipulatenodes.
            /* 2nd condition var. inside global */
            if (targetnode->node_type == prevtype)
            {
                if (prevtype == ast_bool)
                {
                    *returnbool = ((ASTnode_bool *)targetnode)->value;
                    return true;
                }
                else if (prevtype == ast_num)
                {
                    *returnnum = ((ASTnode_num *)targetnode)->value;
                    return true;
                }
            }
        }
        /* should never walk here */
        printf("ID:%s found, but no match on idparamnodes tree.\n", tmpstr.c_str());
        exit(0);
    }
    /* tester */
    // printf("(ast_bool, ast_num) ");
    /* checker */
    printf("Type error!\n");
    exit(0); /* dangerous move */
}

ASTnode *ASTprocess(ASTnode *rootnode, ASTtype prevtype)
{
    // check node
    if (rootnode == NULL)
        return NULL;

    /* pre setting before travel left and right node */
    ASTnode *tmpnode;
    bool ifbool;
    switch (rootnode->node_type)
    {
    case ast_continue:
        rootnode->node_type = prevtype;
        break;
    case ast_root:
        /* prevent stack miss control */
        while (!idparamnodes.empty())
        {
            freenodes(idparamnodes.top());
            idparamnodes.pop();
        }
        break;
    case ast_fun:
        /* ast_fun_continue left tree point to ids, right tree point to exp */
        tmpnode = mallocaddnode(ast_fun_continue, duplicatenodes(rootnode->left), duplicatenodes(rootnode->right));
        freenodes(rootnode);
        return tmpnode;
        break;
    case ast_if:
        ((ASTnode_if *)rootnode)->statement = ASTprocess(((ASTnode_if *)rootnode)->statement, rootnode->node_type);
        returnnode(((ASTnode_if *)rootnode)->statement, ast_bool, &ifbool, NULL);
        /* tester */
        // printf("if->statement: ");
        // printASTtype(((ASTnode_if *)rootnode)->statement);
        if (ifbool)
        {
            /* tester */
            // printf("if->left: ");
            // printASTtype(rootnode->left);
            tmpnode = ASTprocess(duplicatenodes(rootnode->left), rootnode->node_type);
        }
        else
        {
            /* tester */
            // printf("if->right: ");
            // printASTtype(rootnode->right);
            tmpnode = ASTprocess(duplicatenodes(rootnode->right), rootnode->node_type);
        }
        freenodes(rootnode);
        return tmpnode;
        break;
    default:
        break;
    }

    // travel left node
    rootnode->left = ASTprocess(rootnode->left, rootnode->node_type);
    // travel right node
    rootnode->right = ASTprocess(rootnode->right, rootnode->node_type);

    // var. for self node
    int leftnum, rightnum;
    bool leftbool, rightbool;
    // var. for additional fun. call
    ASTnode *expnodes, *tmpidnodes, *targetnode;
    // fun. or id name
    std::string tmpstring;

    /* tester */
    // printASTtype(rootnode);

    // travel self node
    switch (rootnode->node_type)
    {
    case ast_continue:
        /* should never walk here */
        break;
    case ast_root:
        /* no further adjustment needed */
        break;
    case ast_print_bool:
        if (returnnode(rootnode->left, ast_bool, &leftbool, NULL))
        {
            if (leftbool)
                printf("#t\n");
            else
                printf("#f\n");
        }
        freenodes(rootnode);
        break;
    case ast_print_num:
        if (returnnode(rootnode->left, ast_num, NULL, &leftnum))
            printf("%d\n", leftnum);
        freenodes(rootnode);
        break;
    case ast_bool:
        /* no further adjustment needed */
        break;
    case ast_num:
        /* no further adjustment needed */
        break;
    case ast_id:
        returnnode(rootnode, ast_id, &tmpstring);
        if ((!idparamnodes.empty() && findidnode(idparamnodes.top(), tmpstring, &targetnode)) || findmap(0, tmpstring, false, &targetnode))
        {
            rootnode = targetnode;
        }
        break;
    case ast_plus:
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode);
        rootnode = mallocnode(ast_num, leftnum + rightnum);
        break;
    case ast_minus:
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode);
        rootnode = mallocnode(ast_num, leftnum - rightnum);
        break;
    case ast_multiply:
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode);
        rootnode = mallocnode(ast_num, leftnum * rightnum);
        break;
    case ast_divide:
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode);
        rootnode = mallocnode(ast_num, leftnum / rightnum);
        break;
    case ast_greater:
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode);
        rootnode = mallocnode(ast_bool, (leftnum > rightnum) ? true : false);
        break;
    case ast_smaller:
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode);
        rootnode = mallocnode(ast_bool, (leftnum < rightnum) ? true : false);
        break;
    case ast_equal:
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        rightbool = true; // for (rootnode->right)->node_type is ast_num
        // (rootnode->right)->node_type should be ast_num or ast_equal_result
        returnnode(rootnode->right, (rootnode->right)->node_type, &rightbool, &rightnum);
        ifbool = (rightbool && (leftnum == rightnum));
        freenodes(rootnode);
        rootnode = mallocnode(ast_equal_result, ifbool, leftnum);
        break;
    case ast_mod:
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode);
        rootnode = mallocnode(ast_num, leftnum % rightnum);
        break;
    case ast_and:
        returnnode(rootnode->left, ast_bool, &leftbool, NULL);
        returnnode(rootnode->right, ast_bool, &rightbool, NULL);
        freenodes(rootnode);
        rootnode = mallocnode(ast_bool, leftbool && rightbool);
        break;
    case ast_or:
        returnnode(rootnode->left, ast_bool, &leftbool, NULL);
        returnnode(rootnode->right, ast_bool, &rightbool, NULL);
        freenodes(rootnode);
        rootnode = mallocnode(ast_bool, leftbool || rightbool);
        break;
    case ast_not:
        returnnode(rootnode->left, ast_bool, &leftbool, NULL);
        freenodes(rootnode);
        rootnode = mallocnode(ast_bool, !leftbool);
        break;
    case ast_fun:
        /* should never walk here */
        break;
    case ast_fun_continue:
        /* no further adjustment needed */
        break;
    case ast_fun_call:
        /* 0th warning & notification */
        // left node is either fun_exp or fun_name
        // right node is params
        // Every operations here need to do under the dul. nodes, the original nodes cannot be poison.
        if (rootnode->left->node_type == ast_fun_continue) // not ast_fun
        {
            /* 1st fun_exp: anonymous function */
            tmpidnodes = duplicatenodes(rootnode->left->left); // ast_fun_continue left is ids
            expnodes = duplicatenodes(rootnode->left->right);  // ast_fun_continue right is exp
        }
        else if (rootnode->left->node_type == ast_id)
        {
            /* 2nd fun_name: named function */
            returnnode(rootnode->left, ast_id, &tmpstring);
            findmap(1, tmpstring, true, &expnodes);   // use name find exp in index 1
            findmap(2, tmpstring, true, &tmpidnodes); // use name find ids in index 2
        }
        else
        {
            /* checker */
            printf("Type match error!\n");
            exit(0);
        }
        /* pair ids with params */
        tmpidnodes = manipulatenodes(tmpidnodes, duplicatenodes(rootnode->right));
        /* push for recursive function being travel */
        idparamnodes.push(tmpidnodes);
        freenodes(rootnode->left);
        freenodes(rootnode->right);
        /* tester */
        // printf("expnodes type: ");
        // printASTtype(expnodes);
        /* reduce expnodes by calling ASTprocess with given global idparamnodes */
        rootnode = ASTprocess(expnodes, rootnode->node_type);
        /* tester */
        // printf("rootnode: ");
        // printASTtype(rootnode);
        if (!idparamnodes.empty() && ((rootnode->node_type == ast_bool) || (rootnode->node_type == ast_num)))
        {
            freenodes(idparamnodes.top());
            idparamnodes.pop();
        }
        break;
    case ast_fun_body:
        rootnode = rootnode->right; // do not know why it just work
        break;
    case ast_params:
        /* no further adjustment needed */
        break;
    case ast_ids:
        /* no further adjustment needed */
        break;
    case ast_define:
        returnnode(rootnode->left, ast_id, &tmpstring);
        if (rootnode->right->node_type == ast_fun_continue) // not ast_fun
        {
            /* define a function */
            insertmap(1, tmpstring, rootnode->right->right); // index 1: right, exp.
            insertmap(2, tmpstring, rootnode->right->left);  // index 2: left, ids.
        }
        else
        {
            /* define a variable */
            insertmap(0, tmpstring, rootnode->right);
        }
        freenodes(rootnode);
        break;
    case ast_if:
        /* should never walk here */
        break;
    default:
        /* should never walk here */
        break;
    }
    return rootnode;
}

void printASTtype(ASTnode *nownode)
{
    /* tester */
    // return;
    int nownum;
    bool nowbool;
    std::string nowstring;
    ASTtype nowtype = nownode->node_type;
    switch (nowtype)
    {
    case ast_continue:
        /* should never walk here */
        printf("ast_continue\n");
        break;
    case ast_root:
        printf("ast_root\n");
        break;
    case ast_print_bool:
        printf("ast_print_bool\n");
        break;
    case ast_print_num:
        printf("ast_print_num\n");
        break;
    case ast_bool:
        returnnode(nownode, ast_bool, &nowbool, NULL);
        nowstring = nowbool ? "#t" : "#f";
        printf("ast_bool: %s.\n", nowstring.c_str());
        break;
    case ast_num:
        returnnode(nownode, ast_num, NULL, &nownum);
        printf("ast_num: %d.\n", nownum);
        break;
    case ast_id:
        returnnode(nownode, ast_id, &nowstring);
        printf("ast_id: %s.\n", nowstring.c_str());
        break;
    case ast_plus:
        printf("ast_plus\n");
        break;
    case ast_minus:
        printf("ast_minus\n");
        break;
    case ast_multiply:
        printf("ast_multiply\n");
        break;
    case ast_divide:
        printf("ast_divide\n");
        break;
    case ast_greater:
        printf("ast_greater\n");
        break;
    case ast_smaller:
        printf("ast_smaller\n");
        break;
    case ast_equal:
        printf("ast_equal\n");
        break;
    case ast_mod:
        printf("ast_mod\n");
        break;
    case ast_and:
        printf("ast_and\n");
        break;
    case ast_or:
        printf("ast_or\n");
        break;
    case ast_not:
        printf("ast_not\n");
        break;
    case ast_fun:
        printf("ast_fun\n");
        break;
    case ast_fun_continue:
        printf("ast_fun_continue\n");
        break;
    case ast_fun_call:
        printf("ast_fun_call\n");
        break;
    case ast_fun_body:
        printf("ast_fun_body\n");
        break;
    case ast_params:
        printf("ast_params\n");
        break;
    case ast_ids:
        printf("ast_ids\n");
        break;
    case ast_define:
        printf("ast_define\n");
        break;
    case ast_if:
        printf("ast_if\n");
        break;
    default:
        /* should never walk here */
        printf("No match, error!\n");
        break;
    }
    return;
}

void ASTworkhouse(ASTnode *rootnode)
{
    ASTprocess(rootnode, ast_root);
    freemap();
    return;
}
