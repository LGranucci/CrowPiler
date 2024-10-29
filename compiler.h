#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h> 

using namespace std;

struct expression;
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



struct expression{
    LogicalExp* logic;
    expression* next_exp;
    expression() : logic(nullptr),next_exp(nullptr){};
};

struct Statement{
    expression* exp;
    
    bool active;
    Statement() : active(false){};
};

struct Function{
    string name;
    Statement* statement;
    bool active;
    Function() : active(false){};
};
