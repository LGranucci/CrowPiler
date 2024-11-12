
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <map>
#include <set>
#include <string>
#include "compiler.h"

using namespace std;
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
    write_asm(root);
    system("g++ -g -z noexecstack -O0 out.s -o out");
    return 0;
}

