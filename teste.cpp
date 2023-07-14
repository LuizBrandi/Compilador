#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdlib>
#include <vector>
#include <stack>

using namespace std;

typedef struct{
	string varName;
	string type;
	string temp;
} SYMBOL_TYPE;

// string geraIdAleatorio(){
// 	return "var" + to_string(contador++);
// }

unordered_map<string, SYMBOL_TYPE> * criaTabela(){
    return new unordered_map<string, SYMBOL_TYPE>;
}

int main(void){
    // SYMBOL_TYPE teste = {"Guilherme", "cringe", "var1"};
    // SYMBOL_TYPE teste2 = {"Luiz", "cringe", "var0"};
    
    // stack<unordered_map<string, SYMBOL_TYPE> *> pilha;
    // pilha.push(criaTabela());
    // pilha.push(criaTabela());

    // unordered_map<string, SYMBOL_TYPE> * tabelaTopo1 = pilha.top();
    // (*tabelaTopo1)["chave1"] = teste;
    // pilha.pop();

    // unordered_map<string, SYMBOL_TYPE> * tabelaTopo2 = pilha.top();
    // (*tabelaTopo2)["chave1"] = teste2;
    // pilha.pop();
    
    // cout << (*tabelaTopo1)["chave1"].varName + "\n";
    // cout << (*tabelaTopo2)["chave1"].varName + "\n";

    stack<string> pilhaTeste;
    string a = "label1";
    string b;
    b = pilhaTeste.top();
    cout << b;
}

