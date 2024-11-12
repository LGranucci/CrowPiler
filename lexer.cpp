#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <map>
#include <set>
#include <string>
#include "compiler.h"

using namespace std;
std::string alphabet = "abcdefghjkilmnopqrstuwyxvzABCDEFGHJKILMNOPQRSTUWYXVZ";
std::string numbers = "0123456789";


vector<string> lex(ifstream& myread){
    std::string myText;
   
    std::vector<std::string> tokenList;

    while(std::getline(myread, myText)){
        for(int i = 0; i < myText.length(); i++){
            if(myText[i] == '{' || myText[i] == '}' || myText[i] == '(' || myText[i] == ')' || myText[i] == ';' || myText[i] == '-' || myText[i] == '~'  
              || myText[i] == '+' || myText[i] == '/' || myText[i] == '*' || myText[i] == ':' || myText[i] == '?'){
                
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
            else if(myText[i] == 'f' && myText.length() > i + 2){
                std::string aux = myText.substr(i, 3);
                if(aux == "for"){
                    tokenList.push_back("for");
                    i+= 2;
                    continue;
                }
                
            }
            else if(myText[i] == 'w' && myText.length() > i + 4){
                std::string aux = myText.substr(i, 5);
                    if(aux == "while"){
                        tokenList.push_back("while");
                        i+= 4;
                        continue;
                    }
                
            }
            //break
            else if(myText[i] == 'b' && myText.length() > i + 4){
                std::string aux = myText.substr(i, 5);
                if(aux == "break"){
                    tokenList.push_back("break");
                    i+= 4;
                    continue;
                }
            }
            //continue
            else if(myText[i] == 'c' && myText.length() > i + 7){
                std::string aux = myText.substr(i, 8);
                if(aux == "continue"){
                    tokenList.push_back("continue");
                    i+= 6;
                    continue;
                }
                
            }
            //do
            else if(myText[i] == 'd' && myText.length() > i + 1){
                std::string aux = myText.substr(i, 2);
                if(aux == "do"){
                    tokenList.push_back("do");
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
                else{
                    if(myText[i + 1] = 'f'){
                        tokenList.push_back("if");
                        i++;
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
            else if(myText[i] == 'e' && myText.length() > i + 4){
                string aux = myText.substr(i,5);
                if(aux == "else"){
                    tokenList.push_back("else");
                    i+= 4;
                    continue;
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
