#include "compiler.h"
using namespace std;
std::string extern alphabet;
std::string extern numbers;

void pprint_expr(expression* exp);
void pprint_block(BlockItem*);
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
void pprint_cond(ConditionalExp* cond){
    cout<<"COND";
    if(!cond->logic){
        return;
    }
    pprint_or(cond->logic);
    if(cond->exp){
        cout<<" ? ";
        pprint_expr(cond->exp);
        cout<<" : ";
        if(!cond->cond){
            return;
        }
        pprint_cond(cond->cond);
    }
}

void pprint_expr(expression* exp){
    cout<<"EXPRESSION ";
    if(exp->id != ""){
        cout<<exp->id;
        if(!exp->next_exp){
            return;
        }
        pprint_expr(exp->next_exp);
    }
    else{
        pprint_cond(exp->cond);
    }
}

void pprint_statement(Statement* stat){
    cout<<"STATMENT";
    if(stat->isIf){
        cout<<"IF";
        if(!stat->exp){
            cerr<<"no exp";
            return;
        }
        pprint_expr(stat->exp);
        if(!stat->first_if){
            cerr<<"no first if"<<endl;
            return;
        }
        pprint_statement(stat->first_if);
        if(stat->second_if){
            cout<<"ELSE";
            pprint_statement(stat->second_if);

        }
    }
    else if(stat->block){
        cout<<"COMPOUND STAT"<<endl;
        pprint_block(stat->block);
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
void pprint_decl(Declaration* decl){
    cout<<"DECL with id = "<<decl->id;
    
    if(decl->exp){
        pprint_expr(decl->exp);
    }
}


void pprint_block(BlockItem* block){
    cout<<"BLOCK ";
    if(block->stat){
        cout<<"isStatement";
        pprint_statement(block->stat);
    }
    else{
        cout<<"is Declaration";
        pprint_decl(block->decl);
    }
    if(block->next_blockItem)
        pprint_block(block->next_blockItem);
    return;
}
void pretty_printer(Function* root){
    if(root->active){
        cout<<"FUNCTION "<<"\""<<root->name<<"\""<<endl;
    }
    else{
        return;
    }
    
    
    pprint_block(root->statement);

    return;
}


expression* parse_expression(vector<string> tokenList, int& startIndex);
BlockItem* parse_blockItem(vector<string> tokenList, int& startIndex);
//Regola: lo startIndex finisce sull'ultimo carattere della cosa parsata
//si entra in una funzione con lo startIndex rilevante

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
    
    if(tokenList[startIndex][0] == '('){
        startIndex++;
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
        startIndex++;
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
        startIndex++;
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
        startIndex++;
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
        startIndex++;
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
        startIndex+= 2;
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
        startIndex+= 2;
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
ConditionalExp* parse_conditional(vector<string> tokenList, int& startIndex){
    ConditionalExp* cond = new ConditionalExp;
    cond->logic = parse_logical_or(tokenList, startIndex);
    
    if(tokenList[startIndex + 1] != "?"){
        return cond;
    }
    startIndex+= 2;
    cond->exp = parse_expression(tokenList, startIndex);
    if(tokenList[startIndex + 1] == ":"){
        startIndex+= 2;
        cond->cond = parse_conditional(tokenList, startIndex);
        return cond;
    }
    else{
        return nullptr;
    }
}

expression* parse_expression(vector<string> tokenList, int& startIndex){
    //se sono arrivato ad un integer
    expression* exp = new expression;
    if(tokenList[startIndex] == ";"){
        exp->isNull = true;
        return exp;
    }
    if(tokenList.size() >= startIndex + 1 && tokenList[startIndex + 1] == "="){
        exp->id = tokenList[startIndex];
        startIndex += 2;
        exp->next_exp = parse_expression(tokenList,startIndex);
        return exp;
    }

    ConditionalExp* cond = parse_conditional(tokenList, startIndex);
    exp->cond = cond;
    return exp;
    
}
Declaration* parse_declaration(vector<string> tokenList, int& startIndex);
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
    
    if(tokenList[startIndex] == "RETURN_KW"){
        startIndex++;
        stat->isReturn = true;
    }
    if(tokenList[startIndex] == "if"){
        stat->isIf = true;
        if(tokenList[startIndex + 1] != "("){
            return nullptr;
        }
        startIndex+= 2;
        stat->exp = parse_expression(tokenList, startIndex);
        startIndex++;
        if(tokenList[startIndex] != ")" ){
            return nullptr;
        }
        startIndex++;
      
        stat->first_if = parse_statement(tokenList, startIndex);
        if(tokenList[startIndex + 1] == "else"){
            startIndex += 2;
            stat->second_if = parse_statement(tokenList, startIndex); 
        }
        
        return stat;
    }
    if(tokenList[startIndex] == "continue"){
        stat->isContinue = true;
        startIndex++;
        if(tokenList[startIndex] != ";"){
            cout<<"column in wrong position in STATEMENT"<<endl;
        }
        return stat;
    }
    if(tokenList[startIndex] == "break"){
        stat->isBreak = true;
        startIndex++;
        if(tokenList[startIndex] != ";"){
            cout<<"column wrong STAT"<<endl;
        }
        return stat;
    }
    if(tokenList[startIndex] == ";"){
        stat->isNull = true;
        return stat;
    }
    if(tokenList[startIndex] == "for"){
        startIndex++;
        if(tokenList[startIndex] != "(" ){
            cout<<"syntax error in for"<<endl;
        }
        stat->isFor = true;
        startIndex++;
        if(tokenList[startIndex] == "INT_KW"){
            startIndex++;
            stat->for_decl = parse_declaration(tokenList, startIndex);
            startIndex++;
        }
        else{
            stat->exp = parse_expression(tokenList, startIndex);
            startIndex++;
            if(tokenList[startIndex] != ";"){
                cout<<"; in wrong pos"<<endl;
            }
            startIndex++;
        }
        stat->exp2_for = parse_expression(tokenList, startIndex);
        startIndex++;
        if(tokenList[startIndex] != ";"){
            cout<<"; in wrong position"<<endl;
        }
        startIndex++;
        stat->exp3_for = parse_expression(tokenList, startIndex);
        startIndex++;
        if(tokenList[startIndex] != ")"){
            cout<<"no closing bracket in for"<<endl;
        }
        startIndex++;
        stat->next_statement = parse_statement(tokenList, startIndex);
        return stat;
    }
    if(tokenList[startIndex] == "while"){
        stat->isWhile = true;
        startIndex++;
        if(tokenList[startIndex] != "("){
            cout<<"no opening bracket in while"<<endl;
        }
        startIndex++;
        stat->exp = parse_expression(tokenList, startIndex);
        startIndex++;
        if(tokenList[startIndex] != ")"){
            cout<<"no closing bracket"<<endl;
        }
        startIndex++;
        stat->next_statement = parse_statement(tokenList, startIndex);
        return stat;
    }
    if(tokenList[startIndex] == "do"){
        stat->isDo = true;
        startIndex++;
        stat->next_statement = parse_statement(tokenList, startIndex);
        startIndex++;
        if(tokenList[startIndex] != "while"){
            cout<<"error in do"<<endl;
        }
        startIndex++;
        if(tokenList[startIndex] != "("){
            cout<<"missing opening bracket"<<endl;
        }
        startIndex++;
        stat->exp = parse_expression(tokenList, startIndex);
        startIndex++;
        if(tokenList[startIndex] != ")"){
            cout<<"missing closing bracket";
        }
        startIndex++;
        if(tokenList[startIndex] != ";"){
            cout<<"missing ; in while"<<endl;
        }
        return stat;
    }
    if(tokenList[startIndex] == "{"){
        startIndex++;
        BlockItem* block = new BlockItem; 
        block= parse_blockItem(tokenList, startIndex);
        stat->block = block;
        startIndex++;
      
        while(tokenList[startIndex] != "}"){
            
            block = parse_blockItem(tokenList, startIndex);
            startIndex++;
            if(!stat->block->next_blockItem){
                stat->block->next_blockItem = block;
            }
            else{
                BlockItem* aux = stat->block->next_blockItem;
                while(aux->next_blockItem){
                    aux = aux->next_blockItem;
                }
                aux->next_blockItem = block;
            }
        }
        return stat;
    }
    
    
    stat->exp = parse_expression(tokenList, startIndex); 
     
    if(tokenList[startIndex +1] != ";"){
        cout<<"column in wrong position in STATEMENT"<<endl;
    }
    startIndex++;
    stat->active = true;
    return stat;
}
Declaration* parse_declaration(vector<string> tokenList, int& startIndex){
    Declaration* dec = new Declaration;
        
        //for now I just assume it is a valid name
    dec->id = tokenList[startIndex];
    if(tokenList[startIndex + 1] != "="){
        if(tokenList[startIndex + 1] != ";"){
            return nullptr;
        }
        startIndex++;
        return dec;
    }
    startIndex+= 2;
    dec->exp = parse_expression(tokenList, startIndex);
    if(tokenList[startIndex + 1] != ";"){
        cout<<"Errore con il punto e virgola"<<endl;    
    }
    startIndex++;
    return dec;
}


BlockItem* parse_blockItem(vector<string> tokenList, int& startIndex){
    BlockItem* bl = new BlockItem;
    if(tokenList[startIndex] == "INT_KW"){
        startIndex++;
        bl->decl = parse_declaration(tokenList, startIndex);
    }
    else{
        bl->stat = parse_statement(tokenList, startIndex);
    }
    return bl;
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
    root->statement = parse_blockItem(tokenList, index);
    
    
    while(tokenList.size() > index + 1 && tokenList[index + 1] != "}"){
        index++;
        BlockItem* st = new BlockItem;
        st = parse_blockItem(tokenList, index);
        Function* auxexp = new Function;
        
        
        //insert at end of list
        if(!root->statement->next_blockItem){
            root->statement->next_blockItem = st;
        }
        else{
            BlockItem* aux = root->statement->next_blockItem;
            while(aux->next_blockItem){
                aux = aux->next_blockItem;
            }
            aux->next_blockItem = st;
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
