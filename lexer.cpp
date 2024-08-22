#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h> 
using namespace std;

std::string alphabet = "abcdefghjkilmnopqrstuwyxvzABCDEFGHJKILMNOPQRSTUWYXVZ";
std::string numbers = "0123456789";
vector<string> lex(ifstream& myread){
    std::string myText;
   
    std::vector<std::string> tokenList;

    while(std::getline(myread, myText)){
        for(int i = 0; i < myText.length(); i++){
            if(myText[i] == '{' || myText[i] == '}' || myText[i] == '(' || myText[i] == ')' || myText[i] == ';' || myText[i] == '-' || myText[i] == '~'  
              || myText[i] == '+' || myText[i] == '/' || myText[i] == '*'){
                
                string aux(1, myText[i]);
                tokenList.push_back(aux);
                continue;
            }
            else if(myText[i] == '>'){
                if(myText.length() > i + 1 && myText[i+1] == '='){
                    tokenList.push_back(">=");
                    i++;
                    continue;
                }
                else{
                    tokenList.push_back(">");
                    continue;
                }
            }
            else if(myText[i] == '<'){
                if(myText.length() > i + 1 && myText[i+1] == '='){
                    tokenList.push_back("<=");
                    i++;
                    continue;
                }
                else{
                    tokenList.push_back("<");
                    continue;
                }
            }
            else if(myText[i] =='!'){
                if(myText.length() > i + 1 && myText[i+1] == '='){
                    tokenList.push_back("!=");
                    i++;
                    continue;
                }
                else{
                    tokenList.push_back("!");
                    continue;
                }
            }
            else if(myText[i] == '|' && myText.length() > i + 1){
                if(myText[i + 1] == '|'){
                    tokenList.push_back("||");
                    i++;
                    continue;
                }
            }
            else if(myText[i] == '&' && myText.length() > i + 1){
                if(myText[i + 1] == '&'){
                    tokenList.push_back("&&");
                    i++;
                    continue;
                }
            }
            else if(myText[i] == '=' && myText.length() > i + 1){
                if(myText[i + 1] == '='){
                    tokenList.push_back("==");
                    i++;
                    continue;
                }
            }
            else if(myText[i] == 'i' && myText.length() > i + 2){ 
                std::string aux = myText.substr(i, 3);
                if((myText.size() > i + 3 && myText[i + 3] == ' ') || myText.size() == i + 3 ){
                
                    if(aux == "int"){
                        tokenList.push_back("INT_KW");
                        i+= 2;
                        continue;
                    }
                }
            }
            else if(myText[i] == 'r' && myText.length() > i + 5){
                std::string aux = myText.substr(i, 6);
                if((myText.size() > i + 6 && myText[i + 6] == ' ') || myText.size() == i + 6){
                    if(aux == "return"){
                        tokenList.push_back("RETURN_KW");
                        i+= 5;
                        continue;
                    }
                }
            }
            if((alphabet.find(myText[i]) != string::npos)){
                int startI = i;
                for( ; i < myText.length(); i++){
                    if(alphabet.find(myText[i]) == string::npos){
                        break;
                    }
                }
                std::string aux = myText.substr(startI, i - startI);
                i--;
                
                tokenList.push_back(aux);
                continue;
            }
            if((numbers.find(myText[i]) != string::npos)){
                int startI = i;
                 for( ; i < myText.length(); i++){
                    if(numbers.find(myText[i]) == string::npos){
                        break;
                    }
                }
                std::string aux = myText.substr(startI, i - startI);
                i--;
                tokenList.push_back(aux);
                continue;
            }
            
        } 
    }

    return tokenList;
}
struct keyword;
struct expression;

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
struct expression{
    LogicalExp* logic;
    expression* next_exp;
    expression() : logic(nullptr),next_exp(nullptr){};
};

struct Return{
    expression* exp;
    
    bool active;
    Return() : active(false){};
};

struct Function{
    string name;
    keyword* statement;
    bool active;
    Function() : active(false){};
};
struct keyword{
    Return* isReturn;
    //BinaryOp isBinaryOp;
    Constant* isConstant;
    Function* isFunction;
    keyword(): isReturn(nullptr), isConstant(nullptr), isFunction(nullptr){};
};

void pprint_expr(expression* exp);

void pprint_fact(Factor* fact){
    if(fact->exp){
        cout<<"expression inside factor"<<endl;
        pprint_expr(fact->exp);
        return;
    }
    else if(fact->next_fact){
        cout<<fact->un_op<<"FACTOR IN UNOP ";
        pprint_fact(fact->next_fact);
        return;
    }
    else{
        cout<<fact->value<<endl;
        return;
    }

}

void pprint_term(Term* term){
    if(term->op != 'Z')   
        cout<<term->op;
    cout<<"FACTOR ";
    pprint_fact(term->factorList);
    
    if(term->next_term){
  
        pprint_term(term->next_term);
        return;
    }
    return;
}
void pprint_add(AdditiveExp* add){
    cout<<"ADDEXP ";
    if(add->op != 'Z'){
        cout<<add->op;
    }
    if(add->term){
        pprint_term(add->term);
    }
    if(add->next_add){
        pprint_add(add->next_add);
    }
    return;
}
void pprint_rel(RelationalExp* rel){
    cout<<"RELEXP ";
    if(rel->op != "Z"){
        cout<<rel->op;
    }
    if(rel->add){
        pprint_add(rel->add);
    }
    if(rel->next_rel){
        pprint_rel(rel->next_rel);
    }
    return;
}
void pprint_equal(EqualityExp* equal){
    cout<<"EQUALEXP ";
    if(equal->op != 'Z'){
        cout<< equal->op;
    }
    if(equal->rel){
        pprint_rel(equal->rel);
    }
    if(equal->next_eq){
        pprint_equal(equal->next_eq);
    }
    return;
}
void pprint_logic(LogicalExp* logic){
    cout<<"LOGICALEXP ";
    if(logic->equal){
        pprint_equal(logic->equal);
    
    }
    if(logic->next_log){
        cout<<" && ";
        pprint_logic(logic->next_log);

    }
}

void pprint_expr(expression* exp){
    cout<<"EXPRESSION ";
    if(exp->logic){
        pprint_logic(exp->logic);
        if(!exp->next_exp){
            return;
        }
        cout<<" || ";
        pprint_expr(exp->next_exp);
    }
}

void pretty_printer(keyword* root){
    if(root->isFunction->active){
        cout<<"FUNCTION "<<"\""<<root->isFunction->name<<"\""<<endl;
    }
    else{
        return;
    }
    if(!root->isFunction->statement->isReturn->active){
        return;
    }
    Return* ret = root->isFunction->statement->isReturn;
    cout<<"RETURN "<<endl;
    if(!ret->exp){
        cerr<<"expression not present"<<endl;
        return;
    }
    expression* expr = ret->exp;
    pprint_expr(expr);
    
    return;
}


expression* parse_expression(vector<string> tokenList, int& startIndex);


/**
 * @brief parses a token to see if its an unary operator. returns Z if fails.
 */
char get_operator(string token){
    if(token == "!" || token == "~" || token == "-"){
        return token[0];
    }
    else return 'Z';
}

Factor* parse_factor(vector<string> tokenList, int& startIndex){
    Factor* fact = new Factor;
    if(tokenList.size() < startIndex + 1){
        return nullptr;
    }
    startIndex++;
    if(tokenList[startIndex][0] == '('){
        expression* exp = parse_expression(tokenList, startIndex);
        if(!exp) return nullptr;
        startIndex++;
        if(tokenList[startIndex][0] != ')'){
            return nullptr;
        }
        fact->exp = exp;
    }
    //if unary operator
    else if(get_operator(tokenList[startIndex]) != 'Z'){
        char op = get_operator(tokenList[startIndex]);
        Factor* next = parse_factor(tokenList, startIndex);
        fact->un_op = op;
        fact->next_fact = next;
    }
    //if an integer
    else if(numbers.find(tokenList[startIndex][0]) != string::npos){
        fact->value = stoi(tokenList[startIndex]);
    }
    return fact;
}

Term* parse_term(vector<string> tokenList, int& startIndex){
    Term* term = new Term;
    
    Factor* first_factor = parse_factor(tokenList, startIndex);
    term->factorList = first_factor;
    while(tokenList.size() > (startIndex + 1) && (tokenList[startIndex + 1] == "*" || tokenList[startIndex + 1] == "/")){
        startIndex++;
        char op = tokenList[startIndex][0];
        
        first_factor = parse_factor(tokenList, startIndex);
        Term* auxTerm = new Term;
        auxTerm->factorList = first_factor;
        auxTerm->op = op;
        auxTerm->next_term = nullptr;
        if(!term->next_term){
            term->next_term = auxTerm;
        }
        else{
            Term* aux = term->next_term;
            while(aux->next_term){
                aux = aux->next_term;
            }
            aux->next_term = auxTerm;
        }
    }
    return term;
}
AdditiveExp* parse_add(vector<string> tokenList, int& startIndex){
    AdditiveExp* add = new AdditiveExp;
    Term* term = parse_term(tokenList, startIndex);
    add->term = term;
    while(tokenList.size() > (startIndex + 1) && (tokenList[startIndex + 1] == "+" || tokenList[startIndex + 1] == "-")){
        startIndex++;
        char op = tokenList[startIndex][0];
        term = parse_term(tokenList, startIndex);
        AdditiveExp* auxAdd = new AdditiveExp;
        auxAdd->next_add = nullptr;
        auxAdd->term = term;
        auxAdd->op = op;
        if(!add->next_add){
            add->next_add = auxAdd;
        }
        else{
            AdditiveExp* aux = add->next_add;
            while(aux->next_add){
                aux = aux->next_add;
            }
            aux->next_add = auxAdd;
        }       
    }
    return add;
}

RelationalExp* parse_rel(vector<string> tokenList, int& startIndex){
    RelationalExp* rel = new RelationalExp;
    AdditiveExp* add = parse_add(tokenList, startIndex);
    rel->add = add;
    while(tokenList.size() > (startIndex + 1) && (tokenList[startIndex + 1] == "<" || tokenList[startIndex + 1] == ">" || tokenList[startIndex + 1] == "<=" || tokenList[startIndex + 1]== ">=")){
        startIndex++;
        string op = tokenList[startIndex];
        add = parse_add(tokenList, startIndex);
        RelationalExp* auxRel = new RelationalExp;
        auxRel->add = add;
        auxRel->op = op;
        auxRel->next_rel = nullptr;
        if(!rel->next_rel){
            rel->next_rel = auxRel;
        }
        else{
            RelationalExp* aux = rel->next_rel;
            while (aux->next_rel){
                aux = aux->next_rel;
            }
            aux->next_rel = auxRel;
            
        }
    }
    return rel;
}


EqualityExp* parse_equal(vector<string> tokenList, int& startIndex){
    EqualityExp* equal = new EqualityExp;
    
    RelationalExp* rel = parse_rel(tokenList, startIndex);
    equal->rel = rel;
    while(tokenList.size() > (startIndex + 1) && (tokenList[startIndex + 1] == "!=" || tokenList[startIndex + 1] == "==")){
        startIndex++;
        char op = tokenList[startIndex][0];
        rel = parse_rel(tokenList, startIndex);
        EqualityExp* auxEq = new EqualityExp;
        auxEq->rel = rel;
        auxEq->op = op;
        auxEq->next_eq = nullptr;
        if(!equal->next_eq){
            equal->next_eq = auxEq;
        }
        else{
            EqualityExp* aux = equal->next_eq;
            while(aux->next_eq){
                aux = aux->next_eq;
            }
            aux->next_eq = auxEq;
        }
    }
    return equal;
}


LogicalExp* parse_logical(vector<string> tokenList, int& startIndex){
    LogicalExp* logic = new LogicalExp;
    EqualityExp* eq = parse_equal(tokenList, startIndex);
    logic->equal = eq;
    while(tokenList.size() > (startIndex + 1) && (tokenList[startIndex + 1] == "&&")){
        startIndex++;
        eq = parse_equal(tokenList, startIndex);
        LogicalExp* auxlog = new LogicalExp;
        auxlog->equal = eq;
        if(!logic->next_log){
            logic->next_log = auxlog;
        }
        else{
            LogicalExp* aux = logic->next_log;
            while(aux->next_log){
                aux = aux->next_log;
            }
            aux->next_log = auxlog;
        }
    }
    return logic;

}

expression* parse_expression(vector<string> tokenList, int& startIndex){
    //se sono arrivato ad un integer
    expression* exp = new expression;
    LogicalExp* logic = parse_logical(tokenList, startIndex);
    exp->logic = logic;
    while(tokenList.size() > (startIndex + 1) && (tokenList[startIndex + 1] == "||")){
        //there is another logicalExp
        startIndex++;
        logic = parse_logical(tokenList, startIndex);
        
        expression* auxexp = new expression;
        auxexp->logic = logic;
      
        
        //insert at end of list
        if(!exp->next_exp){
            exp->next_exp = auxexp;
        }
        else{
            expression* aux = exp->next_exp;
            while(aux->next_exp){
                aux = aux->next_exp;
            }
            aux->next_exp = auxexp;
        }
    }
    return exp;
}
/**
 * @brief parses a statement, which (for now) can only be of type `return <operation>`
 * @param tokenList list of tokens that the lexer produced
 * @param startIndex index that indicates where to start accessing the vector from
 * 
 */
keyword* parse_statement(vector<string> tokenList, int& startIndex){
    if(tokenList.size()  <= (startIndex + 2)){
        return nullptr;
    }
    if(tokenList[startIndex] != "RETURN_KW"){
        return nullptr;
    }
   
    keyword* stat = new keyword;
    Return* ret = new Return;
    stat->isReturn = ret;
    stat->isReturn->exp = parse_expression(tokenList, startIndex); 
    if(tokenList[startIndex + 1] != ";"){
        return nullptr;
    }
    startIndex+= 1;
    stat->isReturn->active = true;
    return stat;
}

keyword* parse(vector<string> tokenList){
    
    keyword* root = new keyword;
    if(tokenList.size() < 6){
        cout<<"tokenList too small"<<endl;
        return nullptr;
    }
    if(tokenList[0] != "INT_KW"){
        cout<<"function declaration does not start with int"<<endl;
        return nullptr;
    }
    if((alphabet.find(tokenList[1][0]) == string::npos) || tokenList[1] == "INT_KW" || tokenList[1] == "RETURN_KW"){
        cout<< (alphabet.find(tokenList[1]) == string::npos);
        return nullptr;
    }
    if(tokenList[2] != "("){
        cout<<"missing opening bracket"<<endl;
        return nullptr;
    }
    if(tokenList[3] != ")"){
        cout<<"missing closing bracket"<<endl;
        return nullptr;
    }
    if(tokenList[4] != "{"){
        cout<<"missing openinig curly bracket"<<endl;
        return nullptr;
    }
    root->isFunction = new Function;
    root->isFunction->name = tokenList[1];
    root->isFunction->active = true;
    int index = 5;
    root->isFunction->statement = parse_statement(tokenList, index);
    if(!root->isFunction->statement){
        cout<<"function does not contain a valid statement"<<endl;
        return nullptr;
    }
    if(tokenList.size() <= index + 1){
        cout<<"tokenList too small"<<endl;
        return nullptr;

    }
    if(tokenList[index + 1] != "}"){
        cout<<"missing closing bracket"<<endl;
        return nullptr;
    }
    return root;
}
/**
 * @brief generates the level of indentation desired
 * @param indent current level of desired indentation
 */
void generate_indent(int indent, ofstream& outFile){
    for(int i= 0; i < indent; i++){
        outFile<<" ";
    }
}
/*
void write_expression(expression*, ofstream&);
void write_factor(Factor* fact, ofstream& outFile){
    if(!fact->exp && !fact->next_fact && fact->un_op == 'Z'){
        outFile<<"movq $" << fact->value <<", %rax"<<endl;
        return;
    }
    if(fact->exp){
        write_expression(fact->exp, outFile);
    }
    if(fact->next_fact){
        write_factor(fact->next_fact, outFile);
    }
    if(fact->un_op == '-'){
        outFile<<"neg %rax"<<endl;    
    }
    if(fact->un_op == '!'){
        outFile<<"cmpq $0, %rax"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"sete \%al"<<endl;
    }
    if(fact->un_op == '~'){
        outFile<<"not %rax"<<endl;
    }
}

void write_term(Term* term, ofstream& outFile){
    write_factor(term->factorList,outFile );
    if(term->op != 'Z'){
        outFile<<"pop %rcx"<<endl;
    }
    
    if(term->op == '*'){
        outFile<<"imul %rcx"<<endl;
    }
    else if(term->op == '/'){
        outFile<<"movq $0, %rdx"<<endl;
        outFile<<"idiv %rcx"<<endl;
    }
    if(term->next_term){
        outFile<<"push %rax"<<endl;
        write_term(term->next_term, outFile);
    }
}

void write_expression(expression* exp, ofstream& outFile){
    
    write_term(exp->term, outFile);
    if(exp->op != 'Z'){
        outFile<<"pop %rcx"<<endl;
    }
    if(exp->op == '+'){
        outFile<<"addq %rcx, %rax"<<endl;
    }
    else if (exp->op == '-'){
        outFile<<"subq %rax, %rcx"<<endl;
        outFile<<"movq %rcx, %rax"<<endl;
    }
    if(exp->next_exp){
        outFile<<"push %rax"<<endl;
        write_expression(exp->next_exp, outFile);
    }

}

void write_statement(keyword* stat, int indent, ofstream& outFile){
    if(!stat || !stat->isReturn->active){
        return;
    }
    write_expression(stat->isReturn->exp, outFile);
}

void write_asm(keyword* root){
    if(!root){
        return;
    }
    if(!root->isFunction->active){
        cout<<"root does not contain a funcition"<<endl;
        return;
    }
    ofstream outFile("out.s");
    if (!outFile) {
        std::cerr << "Error opening file for writing" << std::endl;
        return;
    }
    //this writes .global _<function name>
    outFile<<" .global " << root->isFunction->name<<endl;
    outFile<<root->isFunction->name<<":"<<endl;

    int indent = 1;
    write_statement(root->isFunction->statement, indent, outFile);
    outFile<<"ret"<<endl;
}
*/



int main(int argc, char *argv[]){
    
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream myread(argv[1]);
    if (!myread.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }
    vector<string> tokenList = lex(myread);
    myread.close();


    for(int i = 0; i < tokenList.size(); i++){
        cout<<tokenList[i]<<endl;
    }
    keyword* root = parse(tokenList);
    if(!root){
        cout<<"\033[1;31merrore nel parser\033[0m\n";
        return EXIT_FAILURE;
    }
    pretty_printer(root);
    //write_asm(root);
    //system("g++ -g out.s -o out");
    return 0;
}

