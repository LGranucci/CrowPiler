#ifndef COMPILER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h> 

using namespace std;

struct expression;
struct BlockItem;
std::string alphabet = "abcdefghjkilmnopqrstuwyxvzABCDEFGHJKILMNOPQRSTUWYXVZ";
std::string numbers = "0123456789";


struct Constant{
    int val;
    bool active;
    Constant() : active(false){};
};

struct Factor{
    expression* exp;
    char un_op;
    Factor* next_fact;
    int value;
    string id;
    Factor() : exp(nullptr),next_fact(nullptr),un_op('Z'){};
};

struct Term{
    Factor* factorList;
    char op;
    Term* next_term;
    Term() : next_term(nullptr),op('Z'){};
};
struct AdditiveExp{
    Term* term;
    AdditiveExp* next_add;
    char op;
    AdditiveExp() : term(nullptr), next_add(nullptr), op('Z'){};
};
struct RelationalExp{
    RelationalExp* next_rel;
    AdditiveExp* add;
    string op;
    RelationalExp() : next_rel(nullptr), add(nullptr), op("Z"){};
};
struct EqualityExp{
    EqualityExp* next_eq;
    RelationalExp* rel;
    char op;
    EqualityExp() : next_eq(nullptr), rel(nullptr), op('Z'){};
};



struct LogicalExp{
    LogicalExp* next_log;
    EqualityExp* equal;
    LogicalExp() : next_log(nullptr), equal(nullptr){};
};



struct LogicalOrExp{
    LogicalOrExp* next_or;
    LogicalExp* and_exp;
    LogicalOrExp() : next_or(nullptr), and_exp(nullptr){};
};

struct ConditionalExp{
    LogicalOrExp* logic;
    expression* exp;
    ConditionalExp* cond;
    ConditionalExp() : logic(nullptr), exp(nullptr), cond(nullptr){};
};

struct expression{
    
    ConditionalExp* cond;
    expression* next_exp;
    string id;
    expression() : cond(nullptr),next_exp(nullptr),id(""){};
};


struct Statement{
    expression* exp;
    Statement* next_statement;
    Statement* first_if;
    Statement* second_if;
    BlockItem* block;
    string id;
    bool isIf;
    bool active;
    bool isReturn;
   
    Statement() : active(false),exp(nullptr),next_statement(nullptr),isReturn(false),id(""),isIf(false), first_if(nullptr), second_if(nullptr), block(nullptr){};

};

struct Declaration{
    string id;
    expression* exp;
    Declaration() : id(""), exp(nullptr){};
};

struct BlockItem{
    Statement* stat;
    Declaration* decl;
    BlockItem* next_blockItem;
    BlockItem() : stat(nullptr), decl(nullptr),next_blockItem(nullptr){};
};
struct Function{
    string name;
    BlockItem* statement;
    bool active;
    Function() : active(false),statement(nullptr){};
};
#endif