#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h> 
#include "compiler.h"
using namespace std;

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
                else{
                    tokenList.push_back("=");
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
    else if(fact->id == ""){
        cout<<fact->value<<endl;
        return;
    }
    else{
        cout<<fact->id<<endl;
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
void pprint_or(LogicalOrExp* orexp){
    cout<<"OR LOGICO ";
    if(orexp->and_exp){
        pprint_logic(orexp->and_exp);
    
    }
    if(orexp->next_or){
        cout<<" || ";
        pprint_or(orexp->next_or);

    }
}


void pprint_expr(expression* exp){
    cout<<"EXPRESSION ";
    if(exp->logic){
        pprint_or(exp->logic);
        if(!exp->next_exp){
            return;
        }
        cout<<" altra exp, con id " << exp->id;
        pprint_expr(exp->next_exp);
    }
}

void pprint_statement(Statement* stat){
    cout<<"STATMENT";
    if(stat->isDeclaration){
            cout<<"DICHIARAZIONE: int" << stat->id;
            if(stat->exp){
                cout<< "= ";
                pprint_expr(stat->exp);
                
            }
    }
    else if(!stat->exp){
        cerr<<"no expression"<<endl;
        return;
    }
    else if(stat->isReturn){
        cout<<"RETURN";
        pprint_expr(stat->exp);
    }
    else{
        cout<<"RIGA";
        pprint_expr(stat->exp);
    }
    
    if(stat->next_statement){
        pprint_statement(stat->next_statement);
    }
    return;
}

void pretty_printer(Function* root){
    if(root->active){
        cout<<"FUNCTION "<<"\""<<root->name<<"\""<<endl;
    }
    else{
        return;
    }
    if(!root->statement->active){
        return;
    }
    
    pprint_statement(root->statement);

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
    else if(numbers.find(tokenList[startIndex - 1][0]) != string::npos){
        fact->value = stoi(tokenList[startIndex - 1]);
    }
    else{
        //sto assumendo che qualunque id sia valido
        fact->id = tokenList[startIndex];
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
LogicalOrExp* parse_logical_or(vector<string> tokenList, int& startIndex){
    LogicalOrExp* or_exp = new LogicalOrExp;
    LogicalExp* and_exp = parse_logical(tokenList, startIndex);
    or_exp->and_exp = and_exp;
    while(tokenList.size() > (startIndex + 1) && (tokenList[startIndex + 1] == "||")){
        //there is another logicalExp
        startIndex++;
        and_exp = parse_logical(tokenList, startIndex);
        
        LogicalOrExp* auxexp = new LogicalOrExp;
        auxexp->and_exp = and_exp;
      
        
        //insert at end of list
        if(!or_exp->next_or){
            or_exp->next_or = auxexp;
        }
        else{
            LogicalOrExp* aux = or_exp->next_or;
            while(aux->next_or){
                aux = aux->next_or;
            }
            aux->next_or = auxexp;
        }
    }
    return or_exp;

}


expression* parse_expression(vector<string> tokenList, int& startIndex){
    //se sono arrivato ad un integer
    expression* exp = new expression;
    if(tokenList.size() >= startIndex + 2 && tokenList[startIndex + 2] == "="){
        exp->id = tokenList[startIndex + 1];
        startIndex += 2;
        exp->next_exp = parse_expression(tokenList,startIndex);
        return exp;
    }
    LogicalOrExp* logic = parse_logical_or(tokenList, startIndex);
    exp->logic = logic;
    return exp;
    
}
/**
 * @brief parses a statement, which (for now) can only be of type `return <operation>`
 * @param tokenList list of tokens that the lexer produced
 * @param startIndex index that indicates where to start accessing the vector from
 * 
 */
Statement* parse_statement(vector<string> tokenList, int& startIndex){
    if(tokenList.size()  <= (startIndex + 2)){
        return nullptr;
    }

    Statement* stat = new Statement;
    stat->isReturn = false;
    stat->isDeclaration = false;
    if(tokenList[startIndex] == "RETURN_KW"){
        //startIndex++?
        stat->isReturn = true;
    }
    if(tokenList[startIndex] == "INT_KW"){
        stat->isDeclaration = true;
        startIndex++;
        
        //for now I just assume it is a valid name
        stat->id = tokenList[startIndex];
        if(tokenList[startIndex + 1] != "="){
            if(tokenList[startIndex + 1] != ";"){
                return nullptr;
            }
            startIndex++;
            stat->active = true;
            return stat;
        }
        startIndex++;
    }
    
    stat->exp = parse_expression(tokenList, startIndex); 
    if(tokenList[startIndex + 1] != ";" && tokenList[startIndex] != ";"){
        return nullptr;
    }
    if(tokenList[startIndex + 1] == ";"){
        startIndex++;
    }
    stat->active = true;
    return stat;
}

Function* parse(vector<string> tokenList){
    
    Function* root = new Function;
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
    
    root->name = tokenList[1];
    root->active = true;
    int index = 5;
    root->statement = parse_statement(tokenList, index);
    
    
    while(tokenList.size() > index + 1 && tokenList[index + 1] != "}"){
        index++;
        Statement* st = new Statement;
        st = parse_statement(tokenList, index);
        Function* auxexp = new Function;
        
        
        //insert at end of list
        if(!root->statement->next_statement){
            root->statement->next_statement = st;
        }
        else{
            Statement* aux = root->statement->next_statement;
            while(aux->next_statement){
                aux = aux->next_statement;
            }
            aux->next_statement = st;
        }
    }

    if(!root->statement){
        cout<<"function does not contain a valid statement"<<endl;
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

//should probably not have global scope
int auxLabel = 0;
string generate_label(string type){
    return type + "_" + to_string(auxLabel++); 
}

/*
void write_expression(expression*,bool, ofstream&);


void write_factor(Factor* fact, ofstream& outFile){
    if(!fact->exp && !fact->next_fact && fact->un_op == 'Z'){
        outFile<<"movq $" << fact->value <<", %rax"<<endl;
        return;
    }
    if(fact->exp){
        write_expression(fact->exp,true, outFile);
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

void write_additive_exp(AdditiveExp* exp, ofstream& outFile){
    
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
    if(exp->next_add){
        outFile<<"push %rax"<<endl;
        write_additive_exp(exp->next_add, outFile);
    }

}
void write_relational_exp(RelationalExp* rexp, ofstream& outFile){
    write_additive_exp(rexp->add, outFile);
    if(rexp->op != "Z"){
        outFile<<"pop %rcx" << endl;
    }
    if(rexp->op == "<"){
        outFile<<"cmpq %rax, %rcx"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"setl \%al"<<endl;
    }
    if(rexp->op == ">"){
        outFile<<"cmpq %rax, %rcx"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"setg \%al"<<endl;
    }
    if(rexp->op == "<="){
        outFile<<"cmpq %rax, %rcx"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"setle \%al"<<endl;
    }
    if(rexp->op == ">="){
        outFile<<"cmpq %rax, %rcx"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"setge \%al"<<endl;
    }

    if(rexp->next_rel){
        write_relational_exp(rexp->next_rel, outFile);
    }

}


void write_equality_exp(EqualityExp* eexp, ofstream& outFile){
    write_relational_exp(eexp->rel, outFile);
    if(eexp->op != 'Z'){
        outFile<<"pop %rcx"<<endl;
    }
    if(eexp->op == '='){
        outFile<<"cmpq %rcx, %rcx"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"sete \%al"<<endl;
    }
    if(eexp->op == '!'){
        outFile<<"cmpq %rcx, %rcx"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"setne \%al"<<endl;
    }
    if(eexp->next_eq){
        outFile<<"push %rax";
        write_equality_exp(eexp->next_eq, outFile);
    }

}

string labelEndAnd;
void write_logic_and(LogicalExp* lexp,bool first, ofstream& outFile){
    write_equality_exp(lexp->equal, outFile);
    if(first && lexp->next_log){
        outFile<< "cmpq $0, %rax"<<endl;
        string label = generate_label("clause");
        outFile<< "jne "<< label<<endl;
        labelEndAnd = generate_label("end");
        outFile<<"jmp " << labelEndAnd<<endl;
        outFile<<label<< ":"<<endl;
    }
    if(!first){
        outFile<<"cmpq $0, %rax"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"setne \%al"<<endl;
        outFile<<labelEndAnd << ':'<<endl;    
    }
    //codice per and logico

    if(lexp->next_log){
        
        write_logic_and(lexp->next_log, !first, outFile);
    }
}

string labelEndOr;
void write_expression(expression* exp,bool first, ofstream& outFile){
    write_logic_and(exp->logic,true, outFile);
    if(first && exp->next_exp){
        outFile<< "cmpq $0, %rax"<<endl;
        string label = generate_label("clause");
        outFile<< "je "<< label<<endl;
        outFile<<"movq $1, %rax"<<endl;
        labelEndOr = generate_label("end");
        outFile<<"jmp " << labelEndOr<<endl;
        outFile<<label<<':'<<endl;
    }
    if(!first){
        outFile<<"cmpq $0, %rax"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"setne \%al"<<endl;
        cout<<"QUI LABEL" << labelEndOr;
        outFile<<labelEndOr << ':'<<endl;    
    }
    
    if(exp->next_exp){
       
        write_expression(exp->next_exp,!first, outFile);
    }
}
void write_statement(Statement* stat, int indent, ofstream& outFile){
    if(!stat || !stat->active){
        return;
    }
    write_expression(stat->exp,true, outFile);
}

void write_asm(Function* root){
    if(!root){
        return;
    }
    if(!root->active){
        cout<<"root does not contain a funcition"<<endl;
        return;
    }
    ofstream outFile("out.s");
    if (!outFile) {
        std::cerr << "Error opening file for writing" << std::endl;
        return;
    }
    //this writes .global _<function name>
    outFile<<" .global " << root->name<<endl;
    outFile<<root->name<<":"<<endl;

    int indent = 1;
    write_statement(root->statement, indent, outFile);
    outFile<<"ret"<<endl;
}
*/



int main(int argc, char *argv[]){
    /*
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    }
*/
    std::ifstream myread(/*argv[1]*/"prova.c");
    
    if (!myread.is_open()) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }
    
    vector<string> tokenList = lex(myread);
    myread.close();


    for(int i = 0; i < tokenList.size(); i++){
        cout<<tokenList[i]<<endl;
    }
    Function* root = parse(tokenList);
    if(!root){
        cout<<"\033[1;31merrore nel parser\033[0m\n";
        return EXIT_FAILURE;
    }
    pretty_printer(root);
    //write_asm(root);
    //system("g++ -g out.s -o out");
    return 0;
}

