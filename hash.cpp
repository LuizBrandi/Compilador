#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdlib>

using namespace std;

typedef struct{
	string varName;
	string type;
} SYMBOL_TYPE;

void insertElement(unordered_map<string, SYMBOL_TYPE>& hash, string key, SYMBOL_TYPE value);
void removeElement(unordered_map<string, SYMBOL_TYPE>& hash, string key);
void findElement(unordered_map<string, SYMBOL_TYPE> hash, string key);
SYMBOL_TYPE returnElement(unordered_map<string, SYMBOL_TYPE>& hash, string key);
void printHash(unordered_map<string, SYMBOL_TYPE> hash);

int main(void){

    unordered_map<string, SYMBOL_TYPE> tabela;
    
    // SYMBOL_TYPE varA = {"Guilherme", "Cringe"};
    // // cout << varA.varName << "\n";

    // SYMBOL_TYPE varB;
    // varB.varName = "Braida";
    // varB.type = "Careca";
    // // cout << varB.varName << "\n";
    
    // SYMBOL_TYPE varC;
    // varC.varName = "Vovô";
    // varC.type = "Garotinho";

    // tabela["t1"] = varA;
    // tabela["t2"] = varB;
    // tabela.insert({"t3", varC});

    insertElement(tabela, "t1", {"Guilherme", "Cringe"});
    insertElement(tabela, "t2", {"Alvim", "E os esquilos"});
    insertElement(tabela, "t3", {"Max", "Mengão"});
    
    // printHash(tabela);

    removeElement(tabela, "t3");

    // printHash(tabela);

    findElement(tabela, "t1");
    SYMBOL_TYPE var = returnElement(tabela, "t1");
    cout << var.varName;
}

void insertElement(unordered_map<string, SYMBOL_TYPE>& hash, string key, SYMBOL_TYPE value){
    hash[key] = value;
}

void removeElement(unordered_map<string, SYMBOL_TYPE>& hash, string key){
        if(hash.find(key) == hash.end()){
            cout << "O elemento não existe ou ja foi removido." << endl;
        }
        hash.erase(key);
}

void findElement(unordered_map<string, SYMBOL_TYPE> hash, string key){
        if(hash.find(key) == hash.end()){
            cout << "O elemento nao existe ou ja foi removido." << endl;
        }else{
            cout << "O elemento esta na hash" << endl;
        }
}

SYMBOL_TYPE returnElement(unordered_map<string, SYMBOL_TYPE>& hash, string key){
    auto iter = hash.find(key);
    if(iter != hash.end()){
        return iter->second;
    } else{
        cout << "O elemento não existe ou ja foi removido." << endl;
        exit(1);
    }
}

void printHash(unordered_map<string, SYMBOL_TYPE> hash){
        for(auto& x: hash){
            std::cout << x.first << " " << x.second.varName << " " << x.second.type << endl;
    }
}



//inserir

//remover

//verificar se uma variável está na hash

//retornar uma variável da hash







