#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

std::string alphabet = "abcdefghjkilmnopqrstuwyxvzABCDEFGHJKILMNOPQRSTUWYXVZ";
std::string numbers = "0123456789";
vector<string> lex(ifstream& myread){
    std::string myText;
   
    std::vector<std::string> tokenList;

    while(std::getline(myread, myText)){
        for(int i = 0; i < myText.length(); i++){
            if(myText[i] == '{' || myText[i] == '}' || myText[i] == '(' || myText[i] == ')' || myText[i] == ';' || myText[i] == '-' || myText[i] == '~' || myText[i] == '!'){
                
                string aux(1, myText[i]);
                tokenList.push_back(aux);
                continue;
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


struct Constant{
    int val;
    bool active;
    Constant() : active(false){};
};
struct expression{
    char op;
    Constant* value;
    expression* next_exp;
    //true if Const is active
    bool opOrInt;
    expression() : opOrInt(true){};
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
    Return isReturn;
    //BinaryOp isBinaryOp;
    Constant isConstant;
    Function isFunction;
};

/**
 * @brief parses a token to see if its an unary operator. returns Z if fails.
 */
char get_operator(string token){
    if(token == "!" || token == "~" || token == "-"){
        return token[0];
    }
    else return 'Z';
}
expression* parse_expression(vector<string> tokenList, int& startIndex){
    //se sono arrivato ad un integer
    expression* newExpr = new expression; 
    if(numbers.find(tokenList[startIndex + 1][0]) != string::npos){

        newExpr->value = new Constant;
        newExpr->opOrInt = true;
        newExpr->value->val = stoi(tokenList[startIndex + 1]);
        newExpr->value->active = true;
        return newExpr;
    }
    else{
        char op = get_operator(tokenList[startIndex + 1]);
        if(op == 'Z'){
            cerr<<"erroneous operator"<<endl;
            return nullptr;
        }
        startIndex++;
        //recursive!
        
        newExpr->next_exp = parse_expression(tokenList, startIndex);
        newExpr->opOrInt = false;
        newExpr->value = nullptr;
        newExpr->op = op;
        return newExpr;
    }
}

keyword* parse_statement(vector<string> tokenList, int& startIndex){
    if(tokenList.size()  <= (startIndex + 2)){
        return nullptr;
    }
    if(tokenList[startIndex] != "RETURN_KW"){
        return nullptr;
    }
   
    keyword* stat = new keyword;
    stat->isReturn.exp = parse_expression(tokenList, startIndex); 
    if(tokenList[startIndex + 2] != ";"){
        return nullptr;
    }
    startIndex+= 2;
    stat->isReturn.active = true;
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
    root->isFunction.name = tokenList[1];
    root->isFunction.active = true;
    int index = 5;
    root->isFunction.statement = parse_statement(tokenList, index);
    if(!root->isFunction.statement){
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

void write_statement(keyword* stat, int indent, ofstream& outFile){
    if(!stat || !stat->isReturn.active){
        return;
    }
    generate_indent(indent, outFile);
    outFile<<"movq $" << stat->isReturn.exp->value->val<<", %rax"<<endl;
    generate_indent(indent, outFile);
    outFile<<"ret"<<endl;
}

void write_asm(keyword* root){
    if(!root){
        return;
    }
    //write asm for function
    if(!root->isFunction.active){
        cout<<"root does not contain a funcition"<<endl;
        return;
    }
    ofstream outFile("out.s");
    if (!outFile) {
        std::cerr << "Error opening file for writing" << std::endl;
        return;
    }
    //this writes .global _<function name>
    outFile<<" .global " << root->isFunction.name<<endl;
    //outFile<<".text"<<endl;
    //outFile<<".global start"<<endl;
    outFile<<root->isFunction.name<<":"<<endl;

    int indent = 1;
    write_statement(root->isFunction.statement, indent, outFile);
}

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
    write_asm(root);
    system("g++ out.s -o out");
    return 0;
}

