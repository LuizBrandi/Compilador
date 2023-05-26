%{
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdlib>
#include <vector>

#define YYSTYPE atributos

using namespace std;

// contador pra gerar ids 
int contador = 1;

struct atributos
{
	string label;
	string traducao;
	string tipo;
};

typedef struct{
	string varName;
	string type;
} SYMBOL_TYPE;

unordered_map<string, SYMBOL_TYPE> SYMBOL_TABLE;

//vector para temporarias
vector<SYMBOL_TYPE> tempList;

int yylex(void);
void yyerror(string);
string geraIdAleatorio();

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
		return SYMBOL_TYPE();
	} 
}

void printHash(unordered_map<string, SYMBOL_TYPE> hash){
        for(auto& x: hash){
            std::cout << x.first << " " << x.second.varName << " " << x.second.type << endl;
    }
}

void printList(vector<SYMBOL_TYPE> list){
	for(int i =0; i < list.size(); i++){
		cout << "\t" << list[i].type << " " << list[i].varName << ";\n";
	}
} 

void insereTempList(string label, string tipo, vector<SYMBOL_TYPE>& tempList){
	cout << "CHEGUEI AQUI\n";
	SYMBOL_TYPE temp;
	temp.varName = label;
	temp.type = tipo;
	tempList.push_back(temp);
	cout << "CHEGUEI AQUI\n";
}

void verificaDeclaracao(string label, string elemento){
	if(!(label == elemento)){
		yyerror("Variável não declarada");
		exit(1);
	}	
}




%}

%token TK_INT TK_FLOAT
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_FLOAT
%token TK_FIM TK_ERROR;



%start S

%left '+' '-'
%left '*' '/'

%%

S 			: TK_TIPO_INT TK_MAIN '(' ')' BLOCO
			{
				cout << "/*Compilador FOCA*/\n" << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\nint main(void)\n{\n";
				printList(tempList); 
				cout << $5.traducao << "\treturn 0;\n}" << endl; 
			}
			;

BLOCO		: '{' COMANDOS '}'
			{
				$$.traducao = $2.traducao;
			}
			;

COMANDOS	: COMANDO COMANDOS
			{
				$$.traducao =  $1.traducao + $2.traducao;
			}
			|
			{
				$$.traducao = "";
			}
			;

COMANDO 	: E ';'
			| TK_TIPO_INT TK_ID ';'
			{

				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "int";
				//insere id na tabela de simbolos
				insertElement(SYMBOL_TABLE, $2.label, value);

				$$.traducao = "";
				$$.label = "";
			}
			| TK_TIPO_FLOAT TK_FLOAT ';'
			{

				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "int";
				//insere id na tabela de simbolos
				insertElement(SYMBOL_TABLE, $2.label, value);

				$$.traducao = "";
				$$.label = "";
			}		
			//$1	$2	($3 --> 1 + 1);
			| TK_ID '=' E ';'
			{
			
				$$.traducao = $1.traducao + $3.traducao + "\t" + $1.label + " = " +
														$3.label + ";\n";
			}
			;

E 			: E '+' E
			{					//var3			var4		
			
				$$.label = geraIdAleatorio();			
				$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
							$1.label + " + " + $3.label + ";\n";
			}
			| E '-' E
			{
				$$.label = geraIdAleatorio();			
				$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
				$1.label + " - " + $3.label + ";\n";
			}
			| E '*' E
			{
				$$.label = geraIdAleatorio();			
				$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
				$1.label + " * " + $3.label + ";\n";
			}
			| E '/' E
			{
				$$.label = geraIdAleatorio();			
				$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
				$1.label + " / " + $3.label + ";\n";
			}
			| TK_INT
			{
				$$.tipo = "int";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, tempList);
				//
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";

			}
			| TK_FLOAT
			{
				$$.tipo = "float";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, tempList);
				//
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";

			}
			| TK_ID
			{
		
				//procurando elemento na hash
				SYMBOL_TYPE elemento = returnElement(SYMBOL_TABLE, $1.label);

				verificaDeclaracao($1.label, elemento.varName);
				$$.label =  geraIdAleatorio();
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";
			}
			;
%%


#include "lex.yy.c"

int yyparse();

int main( int argc, char* argv[] )
{
	yyparse();

	return 0;
}

void yyerror( string MSG )
{
	cout << MSG << endl;
	exit(0);
}				

string geraIdAleatorio(){
	return "var" + to_string(contador++);
}


