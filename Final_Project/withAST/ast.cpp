#include "ast.h"

ASTnode *mallocaddnode(ASTtype newtype, ASTnode *newleft, ASTnode *newright)
{
    ASTnode *newnode = (ASTnode *)malloc(sizeof(ASTnode));
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
        newnode->name = newname;
        return (ASTnode *)newnode;
    }
    return NULL;
}

void freenodes(ASTnode *nownode, int freenum)
{
    /*
    just for testing
    */
    return;
    if (freenum >= 4)
    {
        free(((ASTnode_if *)nownode)->statement);
    }
    if (freenum >= 3)
    {
        free(nownode->right);
    }
    if (freenum >= 2)
    {
        free(nownode->left);
    }
    if (freenum >= 1)
    {
        free(nownode);
    }
    return;
}

bool checknode(ASTnode *nownode)
{
    if (nownode != NULL)
    {
        return true;
    }
    printf("AST error!\n");
    return false; /* NULL */
}

bool returnnode(ASTnode *nownode, ASTtype prevtype, bool *returnbool, int *returnnum)
{
    if ((nownode != NULL) && (nownode->node_type == prevtype))
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
    }
    printf("Type error!\n");
    /*
    just for testing
    */
    // printf("%d, %d.\n", nownode->node_type, prevtype);
    return false;
}

ASTnode *ASTprocess(ASTnode *rootnode, ASTtype prevtype)
{
    // check node
    if (rootnode == NULL)
    {
        return NULL;
    }
    /*
    set node type
    */
    if (rootnode->node_type == ast_continue)
    {
        rootnode->node_type = prevtype;
    }
    // travel left node
    rootnode->left = ASTprocess(rootnode->left, rootnode->node_type);
    // travel right node
    rootnode->right = ASTprocess(rootnode->right, rootnode->node_type);
    // travel self node
    switch (rootnode->node_type)
    {
        int leftnum, rightnum;
        bool leftbool, rightbool;
    case ast_continue:
        /*
        should never walk here
        */
        printf("ast_continue\n");
        break;
    case ast_root:
        printf("ast_root\n");
        break;
    case ast_print_bool:
        printf("ast_print_bool\n");
        if (!checknode(rootnode->left))
            break;
        bool printbool;
        if (returnnode(rootnode->left, ast_bool, &printbool, NULL))
        {
            if (printbool)
            {
                printf("#t\n");
            }
            else
            {
                printf("#f\n");
            }
        }
        break;
    case ast_print_num:
        printf("ast_print_num\n");
        if (!checknode(rootnode->left))
            break;
        int printnum;
        if (returnnode(rootnode->left, ast_num, NULL, &printnum))
        {
            printf("%d\n", printnum);
        }
        break;
    case ast_bool:
        printf("ast_bool\n");
        break;
    case ast_num:
        printf("ast_num\n");
        break;
    case ast_id:
        printf("ast_id\n");
        break;
    case ast_plus:
        printf("ast_plus\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode, 3);
        rootnode = mallocnode(ast_num, leftnum + rightnum);
        break;
    case ast_minus:
        printf("ast_minus\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode, 3);
        rootnode = mallocnode(ast_num, leftnum - rightnum);
        break;
    case ast_multiply:
        printf("ast_multiply\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode, 3);
        rootnode = mallocnode(ast_num, leftnum * rightnum);
        break;
    case ast_divide:
        printf("ast_divide\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode, 3);
        rootnode = mallocnode(ast_num, leftnum / rightnum);
        break;
    case ast_greater:
        printf("ast_greater\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode, 3);
        rootnode = mallocnode(ast_bool, (leftnum > rightnum) ? true : false);
        break;
    case ast_smaller:
        printf("ast_smaller\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode, 3);
        rootnode = mallocnode(ast_bool, (leftnum < rightnum) ? true : false);
        break;
    case ast_equal:
        printf("ast_equal\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, (rootnode->right)->node_type, &rightbool, &rightnum);
        if ((((rootnode->right)->node_type == ast_bool) && (!rightbool)) || (leftnum != rightnum))
        {
            freenodes(rootnode, 3);
            rootnode = mallocnode(ast_bool, false);
        }
        else
        {
            freenodes(rootnode, 3);
            rootnode = mallocnode(ast_num, leftnum);
        }
        break;
    case ast_mod:
        printf("ast_mod\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_num, NULL, &leftnum);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, ast_num, NULL, &rightnum);
        freenodes(rootnode, 3);
        rootnode = mallocnode(ast_num, leftnum % rightnum);
        break;
    case ast_and:
        printf("ast_and\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_bool, &leftbool, NULL);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, ast_bool, &rightbool, NULL);
        freenodes(rootnode, 3);
        rootnode = mallocnode(ast_bool, leftbool && rightbool);
        break;
    case ast_or:
        printf("ast_or\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_bool, &leftbool, NULL);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, ast_bool, &rightbool, NULL);
        freenodes(rootnode, 3);
        rootnode = mallocnode(ast_bool, leftbool || rightbool);
        break;
    case ast_not:
        printf("ast_not\n");
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, ast_bool, &leftbool, NULL);
        freenodes(rootnode, 2);
        rootnode = mallocnode(ast_bool, !leftbool);
        break;
    case ast_fun:
        printf("ast_fun\n");
        break;
    case ast_fun_call:
        printf("ast_fun_call\n");
        break;
    case ast_ids:
        printf("ast_ids\n");
        break;
    case ast_define:
        printf("ast_define\n");
        break;
    case ast_if:
        printf("ast_if\n");
        if (!checknode(((ASTnode_if *)rootnode)->statement))
            break;
        ((ASTnode_if *)rootnode)->statement = ASTprocess(((ASTnode_if *)rootnode)->statement, rootnode->node_type);
        bool ifbool;
        returnnode(((ASTnode_if *)rootnode)->statement, ast_bool, &ifbool, NULL);
        if (!checknode(rootnode->left))
            break;
        returnnode(rootnode->left, (rootnode->left)->node_type, &leftbool, &leftnum);
        if (!checknode(rootnode->right))
            break;
        returnnode(rootnode->right, (rootnode->right)->node_type, &rightbool, &rightnum);
        if (ifbool)
        {
            if ((rootnode->left)->node_type == ast_num)
            {
                rootnode = mallocnode(ast_num, leftnum);
            }
            else /* should be ast_bool */
            {
                rootnode = mallocnode(ast_bool, leftbool);
            }
            freenodes(rootnode, 4);
        }
        else
        {
            if ((rootnode->right)->node_type == ast_num)
            {
                rootnode = mallocnode(ast_num, rightnum);
            }
            else /* should be ast_bool */
            {
                rootnode = mallocnode(ast_bool, rightbool);
            }
            freenodes(rootnode, 4);
        }
        break;
    default:
        /*
        should never walk here
        */
        printf("No match\n");
        break;
    }
    return rootnode;
}
