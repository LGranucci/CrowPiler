#include "compiler.h"


using namespace std;   

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
;

void write_expression(expression*,bool, ofstream&, map<std::string, int>& var_map, int& stackIndex);
void write_block(BlockItem*, ofstream&, map<std::string, int>& var_map, int& stackIndex, set<string>& current_set);

void write_factor(Factor* fact, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex){
    if(!fact->exp && !fact->next_fact && fact->un_op == 'Z' && fact->id == ""){
        outFile<<"movq $" << fact->value <<", %rax"<<endl;
        return;
    }
    else if(fact->id != ""){
        int var_offset = var_map[fact->id];
        if(var_offset == 0){
            cerr<<"ERROR: undeclared variable"<<endl;
            exit(0);
        }
        outFile<<"movq "<<var_offset<<"(\%rbp), \%rax"<<endl;
    }

    if(fact->exp){
        write_expression(fact->exp,true, outFile, var_map, stackIndex);
    }
    if(fact->next_fact){
        write_factor(fact->next_fact, outFile, var_map, stackIndex);
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

void write_term(Term* term, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex){
    write_factor(term->factorList,outFile, var_map, stackIndex);
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
        write_term(term->next_term, outFile, var_map, stackIndex);
    }
}

void write_additive_exp(AdditiveExp* exp, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex){
    
    write_term(exp->term, outFile, var_map, stackIndex);
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
        write_additive_exp(exp->next_add, outFile, var_map, stackIndex);
    }

}
void write_relational_exp(RelationalExp* rexp, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex){
    write_additive_exp(rexp->add, outFile, var_map, stackIndex);
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
        write_relational_exp(rexp->next_rel, outFile, var_map, stackIndex);
    }

}


void write_equality_exp(EqualityExp* eexp, ofstream& outFile, map<std::string, int> var_map, int& stackIndex){
    write_relational_exp(eexp->rel, outFile, var_map, stackIndex);
    if(eexp->op != 'Z'){
        outFile<<"pop %rcx"<<endl;
    }
    if(eexp->op == '='){
        outFile<<"cmpq %rax, %rcx"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"sete \%al"<<endl;
    }
    if(eexp->op == '!'){
        outFile<<"cmpq %rax, %rcx"<<endl;
        outFile<<"movq $0, %rax"<<endl;
        outFile<<"setne \%al"<<endl;
    }
    if(eexp->next_eq){
        outFile<<"push %rax"<<endl;
        write_equality_exp(eexp->next_eq, outFile, var_map, stackIndex);
    }

}

string labelEndAnd;
void write_logic_and(LogicalExp* lexp,bool first, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex){
    write_equality_exp(lexp->equal, outFile, var_map, stackIndex);
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
        
        write_logic_and(lexp->next_log, !first, outFile, var_map, stackIndex);
    }
}
string labelEndOr;

void write_logic_or(LogicalOrExp* exp, bool first, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex){
    write_logic_and(exp->and_exp,true, outFile, var_map, stackIndex);
    if(first && exp->next_or){
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
    
    if(exp->next_or){
       
        write_logic_or(exp->next_or,!first, outFile, var_map, stackIndex);
    }
}

void write_conditional_exp(ConditionalExp* cond, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex){
    write_logic_or(cond->logic, true, outFile, var_map, stackIndex);
    if(cond->exp){
        outFile<<"cmpq $0, %rax"<<endl;
        string lab = generate_label("e");
        outFile<<"je "<< lab<<endl;
        write_expression(cond->exp, true, outFile, var_map, stackIndex);
        string post = generate_label("post_conditional");
        outFile<<"jmp "<<post<<endl;

        outFile<<lab<<":"<<endl;
        write_conditional_exp(cond->cond, outFile, var_map, stackIndex);
        outFile<<post<<":"<<endl;
    }
}

void write_expression(expression* exp,bool first, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex){

    if(exp->id != ""){
        if(var_map.find(exp->id) == var_map.end()){
            cerr<<"variabile non dichiarata"<<endl;
            return;
        }
        int var_offset = var_map[exp->id];
        write_expression(exp->next_exp, true, outFile, var_map, stackIndex);
       
        outFile<<"movq \%rax, "<<var_offset<<"(\%rbp)"<<endl;
        return;
    }
    else{
        write_conditional_exp(exp->cond, outFile, var_map, stackIndex);
    }
    
}
void write_statement(Statement* stat, int indent, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex){
    if(!stat){
        return;
    }
    if(stat->isIf){
        write_expression(stat->exp, true, outFile, var_map, stackIndex);
        outFile<<"cmpq $0, %rax"<<endl;
        
        string labelElse = generate_label("else");
        outFile<<"je "<<labelElse<<endl;
        write_statement(stat->first_if, 0, outFile, var_map, stackIndex);
        string post = generate_label("post");
        outFile<<"jmp " <<post<<endl;
        outFile<<labelElse<<":"<<endl;
        if(stat->second_if){
            write_statement(stat->second_if, 0, outFile, var_map, stackIndex);
        }
        outFile<<post<<":"<<endl;
    }
    else if(stat->block){
        //new block: have to take care of the variables
        //should: duplicate var map, create a current_set of allocated variables in the block only
        std::set<string> current_set;
        map<string, int> new_map(var_map);
        int new_index = stackIndex;
        write_block(stat->block, outFile, new_map, new_index, current_set);
        //end of block: deallocating variables
        int to_dealloc = current_set.size() * 8;
        outFile<<"addq $" <<to_dealloc<<", %rsp"<<endl;

    }
    else{
        write_expression(stat->exp,true, outFile, var_map, stackIndex);
        if(stat->isReturn){
           outFile<<"movq %rbp, %rsp"<<endl;
            outFile<<"pop %rbp"<<endl;
            outFile<<"ret"<<endl;
        }
    }
   
}

void write_decl(Declaration* decl, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex, std::set<string>& current_set){
        if(current_set.find(decl->id) != current_set.end()){
            cerr<<"var già dichiarata"<<endl;
            return;
        }
        if(decl->exp){
            write_expression(decl->exp, true, outFile, var_map, stackIndex);
        }
        else{
            outFile<<"movq $0, \%rax"<<endl;
        }
        outFile<<"push \%rax"<<endl;
        stackIndex -= 8;
        current_set.insert(decl->id);
        var_map[decl->id] = stackIndex;
}


void write_block(BlockItem* block, ofstream& outFile, map<std::string, int>& var_map, int& stackIndex, std::set<string>& current_set){
    if(block->decl){
        write_decl(block->decl, outFile, var_map, stackIndex, current_set);
    }
    else{
        write_statement(block->stat, 0, outFile, var_map, stackIndex);
    }
    if(block->next_blockItem){
        write_block(block->next_blockItem, outFile, var_map, stackIndex, current_set);
    }
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
    int stackIndex = 0;
    map<std::string, int> var_map;
    set<string> current_set;
    //this writes .global _<function name>
    outFile<<" .global " << root->name<<endl;
    outFile<<root->name<<":"<<endl;

    int indent = 1;
    outFile<<"push %rbp"<<endl;
    outFile<<"movq %rsp, %rbp"<<endl;
    
    write_block(root->statement, outFile, var_map, stackIndex, current_set);
    outFile<<"movq %rbp, %rsp"<<endl;
    outFile<<"pop %rbp"<<endl;
    outFile<<"ret"<<endl;
}



