#ifndef _AST_H_
#define _AST_H_

#include <iostream>
#include <map>
#include <string>

enum ASTtype
{
    ast_continue,
    ast_root,
    ast_print_bool,
    ast_print_num,
    ast_bool,
    ast_num,
    ast_id,
    ast_plus,
    ast_minus,
    ast_multiply,
    ast_divide,
    ast_greater,
    ast_smaller,
    ast_equal,
    ast_mod,
    ast_and,
    ast_or,
    ast_not,
    ast_fun,
    ast_fun_continue,
    ast_fun_call,
    ast_params,
    ast_ids,
    ast_define,
    ast_if,
};

class ASTnode
{
public:
    ASTtype node_type;
    ASTnode *left = NULL, *right = NULL;
};

class ASTnode_bool : public ASTnode
{
public:
    bool value;
};

class ASTnode_num : public ASTnode
{
public:
    int value;
};

class ASTnode_id : public ASTnode
{
public:
    std::string name;
    ASTnode *target;
};

class ASTnode_if : public ASTnode
{
public:
    ASTnode *statement;
};

int manipulatemaplength();

bool insertmap(int index, std::string insertname, ASTnode *insertnode);
bool findmap(int index, std::string findname, ASTnode **targetnode);

ASTnode *duplicatenodes(ASTnode *rootnode);

ASTnode *mallocaddnode(ASTtype newtype, ASTnode *newleft, ASTnode *newright);
ASTnode *mallocaddnode(ASTtype newtype, ASTnode *newleft, ASTnode *newright, ASTnode *newstatement);

ASTnode *mallocnode(ASTtype newtype, bool newvalue);
ASTnode *mallocnode(ASTtype newtype, int newvalue);
ASTnode *mallocnode(ASTtype newtype, std::string newname);

void freenodes(ASTnode *nownode, int freenum);

bool checknode(ASTnode *nownode);
bool returnnode(ASTnode *nownode, ASTtype prevtype, std::string *returnstring);
bool returnnode(ASTnode *nownode, ASTtype prevtype, bool *returnbool, int *returnnum);

// ASTnode *ASTprocess(ASTnode *rootnode, ASTtype prevtype);
ASTnode *ASTprocess(ASTnode *rootnode, ASTtype prevtype, std::string targetfunction, ASTnode *paramnodes);
void printASTtype(ASTtype nowtype);

#endif /* _AST_H_ */
