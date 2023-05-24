%{
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdlib>

#define YYSTYPE atributos

using namespace std;

// contador pra gerar ids 
int contador = 1;

struct atributos
{
	string label;
	string batata;
	string traducao;
	string tipo;
};

typedef struct{
	string varName;
	string type;
} SYMBOL_TYPE;


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

unordered_map<string, SYMBOL_TYPE> SYMBOL_TABLE;

int yylex(void);
void yyerror(string);
string geraIdAleatorio();
%}

%token TK_INT
%token TK_MAIN TK_ID TK_TIPO_INT
%token TK_FIM TK_ERROR;


%start S

%left '+' '-'
%left '*' '/'

%%

S 			: TK_TIPO_INT TK_MAIN '(' ')' BLOCO
			{
				cout << "/*Compilador FOCA*/\n" << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\nint main(void)\n{\n" << $5.traducao << "\treturn 0;\n}" << endl; 
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
				// cout << $$.label;
				$$.label =  geraIdAleatorio();
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";
				// $$.batata = "\t" + $$.tipo + " " + $1.label + ";\n";
			}
			| TK_ID
			{
		
				//procurando elemento na hash
				SYMBOL_TYPE elemento = returnElement(SYMBOL_TABLE, $1.label);

				if(!($1.label == elemento.varName)){
					yyerror("Variável não declarada");
					exit(1);
				}

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


