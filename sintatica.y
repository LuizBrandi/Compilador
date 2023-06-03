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
int contador = 0;
bool sinalizaConversao = 1;


struct atributos
{
	string label;
	string traducao;
	string tipo;
};

typedef struct{
	string varName;
	string type;
	string temp;
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

bool findElement(unordered_map<string, SYMBOL_TYPE> hash, string key){
        if(hash.find(key) == hash.end()){
            cout << "O elemento nao existe ou ja foi removido." << endl;
			return false;
        }else{
            cout << "O elemento esta na hash" << endl;
			return true;
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
	SYMBOL_TYPE temp;
	temp.varName = label;
	temp.type = tipo;
	tempList.push_back(temp);
}

void realizaOperacao(atributos& $$, atributos& $1, atributos& $3, string operacao){
	//criando temporaria que recebera a soma
	SYMBOL_TYPE value;

	int caso = 0;

	//1° caso -> int e int					
	if($1.tipo == "int" && $3.tipo == "int"){
		$$.tipo = "int";
		value.type = "int";
		caso = 0;
	} 
	// 2° caso -> float e float
	if($1.tipo == "float" && $3.tipo == "float" ){
		$$.tipo = "float";
		value.type = "float";
		caso = 0;
	} 
	
	// 3° caso -> int e float
	if($1.tipo == "int" && $3.tipo == "float" ){
		$$.tipo = "float";
		value.type = "float";
		caso = 3;
	}
	// 4° caso -> float e int					
	if($1.tipo == "float" && $3.tipo == "int" ){
		$$.tipo = "float";
		value.type = "float";
		caso = 4;
	}
	
	//1° caso -> int e int		
	if(caso == 0){
		caso = 0;
		$$.label = geraIdAleatorio();	
		value.varName = $$.label;
		$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
				$1.label + operacao + $3.label + ";\n";	
	}

	// conversao int e float
	if(caso == 3){
		caso = 0;
		//Criando var de conversão implicita  e inserindo na lista de temps
		SYMBOL_TYPE tempConvert;
		tempConvert.temp = geraIdAleatorio();
		tempConvert.type = "float";
		insereTempList(tempConvert.temp, tempConvert.type, tempList);
		//Criando o label da var que vai receber a conversão
		$$.label = geraIdAleatorio();	
		value.varName = $$.label;
		
		$$.traducao = $1.traducao + $3.traducao + 
		"\t" + tempConvert.temp +  " = " + "(float)" + $1.label + ";\n"
		+ "\t" + $$.label + " = " +  $3.label + operacao + tempConvert.temp + ";\n";
	}
	// conversao float e int
	if(caso == 4){
		caso = 0;
		//Criando var de conversão implicita  e inserindo na lista de temps
		SYMBOL_TYPE tempConvert;
		tempConvert.temp = geraIdAleatorio();
		tempConvert.type = "float";
		insereTempList(tempConvert.temp, tempConvert.type, tempList);

		$$.label = geraIdAleatorio();	
		value.varName = $$.label;

		$$.traducao = $1.traducao + $3.traducao + 
		"\t" + tempConvert.temp +  " = " + "(float)" + $3.label + ";\n"
		+ "\t" + $$.label + " = " +  $1.label + operacao + tempConvert.temp + ";\n";
	}	


	value.temp = $$.label;				
	insereTempList(value.temp, value.type, tempList);
}

void verificaDeclaracao(string label, string elemento){
	if(!(label == elemento)){
		yyerror("Variável não declarada");
		exit(1);
	}	
}


%}

%token TK_INT TK_FLOAT TK_BOOL TK_TIPO_BOOL TK_TRUE TK_FALSE
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_FLOAT
%token TK_FIM TK_ERROR
%token TK_GREATER_EQUAL TK_LESS_EQUAL TK_EQUAL_EQUAL TK_NOT_EQUAL
%token TK_AND TK_OR TK_NOT


%start S

%left '+' '-'
%left '*' '/'
%left TK_AND TK_OR
%left TK_LESS_EQUAL TK_GREATER_EQUAL
%left '>' '<' 

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
				value.temp = geraIdAleatorio();

				//insere id na tabela de simbolos
				insertElement(SYMBOL_TABLE, $2.label, value);
				insereTempList(value.temp, value.type, tempList);
				$$.traducao = $1.traducao + $2.traducao;
			}
			| TK_TIPO_FLOAT TK_ID ';'
			{
				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "float";
				value.temp = geraIdAleatorio();
				
				//insere id na tabela de simbolos
				insertElement(SYMBOL_TABLE, $2.label, value);
				insereTempList(value.temp, value.type, tempList);
				$$.traducao = $1.traducao + $2.traducao;
			}
			| TK_TIPO_BOOL TK_ID ';'
			{
				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "int";
				value.temp = geraIdAleatorio();
				
				//insere id na tabela de simbolos
				insertElement(SYMBOL_TABLE, $2.label, value);
				insereTempList(value.temp, value.type, tempList);
				$$.traducao = $1.traducao + $2.traducao;
			}
			| TK_ID '=' TK_INT ';'
			{
				if(findElement(SYMBOL_TABLE, $1.label)){
					$$.traducao = $1.traducao + $3.traducao + "\t" + SYMBOL_TABLE[$1.label].temp + " = " +
														$3.label + ";\n";
				}else{
					exit(1);
				}	
			}		
			| TK_ID '=' TK_FLOAT ';'
			{
				if(findElement(SYMBOL_TABLE, $1.label)){
					$$.traducao = $1.traducao + $3.traducao + "\t" + SYMBOL_TABLE[$1.label].temp + " = " +
														$3.label + ";\n";
				}else{
					exit(1);
				}		
			}
			| TK_ID '=' TK_TRUE ';'
			{
				if(findElement(SYMBOL_TABLE, $1.label)){
					$$.traducao = $1.traducao + $3.traducao + "\t" + SYMBOL_TABLE[$1.label].temp + " = " +
														"1" + ";\n";
				}else{
					exit(1);
				}		
			}			
			| TK_ID '=' TK_FALSE ';'
			{
				if(findElement(SYMBOL_TABLE, $1.label)){
					$$.traducao = $1.traducao + $3.traducao + "\t" + SYMBOL_TABLE[$1.label].temp + " = " +
														"0" + ";\n";
				}else{
					exit(1);
				}		
			}			
			| TK_ID '=' E ';'
			{
				if(findElement(SYMBOL_TABLE, $1.label)){
					SYMBOL_TYPE value = returnElement(SYMBOL_TABLE, $1.label);

					//Se o tipo do TK_ID for igual ao tipo da Expressão, não alteramos o tipo da Expressão, atribuindo normalmente
					if((value.type == $3.tipo)){
						$$.traducao = $1.traducao + $3.traducao + "\t" + SYMBOL_TABLE[$1.label].temp + " = " 
						+ $3.label + ";\n";
					}
									
					//Acima verificamos se o tipo é igual, se for, não alteramos em nada.

					/*Entretanto, se eles forem diferentes, eu tenho que verificar se o 
					tipo do primeiro e do segundo é BOOL, se for, atribuimos normalmente, mas
					se eles não forem BOOL, fazemos a conversão de tipo. 
					*/

					//Se o tipo do TK_ID for diferente da Expressão, mudamos o tipo da Expressão antes da atribuição
					else{
							$$.traducao = $1.traducao + $3.traducao + "\t" + SYMBOL_TABLE[$1.label].temp + " = " + 
							"(" + value.type + ")" + $3.label + ";\n";			
					}
				}else{
					exit(1);
				}
			}
			;

E 			: E '+' E
			{		
				realizaOperacao($$, $1, $3, " + ");
			}
			| E '-' E
			{
				realizaOperacao($$, $1, $3, " - ");
			}
			| E '*' E
			{
				realizaOperacao($$, $1, $3, " * ");
			}
			| E '/' E
			{
				realizaOperacao($$, $1, $3, " / ");
			}
			| '(' TIPO ')' E 
			{
				$$.tipo = "float";
				$$.label =  geraIdAleatorio();
				//sinalizaConversao comeca true
				cout << "Valor do sinaliza conversao:" << (sinalizaConversao) << "\n";
				sinalizaConversao = 0;
				cout << "Valor do sinaliza conversao:" << (sinalizaConversao) << "\n";
				SYMBOL_TYPE tempConvert;
				tempConvert.temp = $$.label;
				tempConvert.type = "float";
				insereTempList(tempConvert.temp, tempConvert.type, tempList);

				$$.traducao = $2.traducao + $4.traducao + "\t" + tempConvert.temp + " = " + "(" + $$.tipo + ")" + $4.label + ";\n";

			}
			| E '>' E
			{
				realizaOperacao($$, $1, $3, " > ");
			}
			| E TK_GREATER_EQUAL E
			{
				realizaOperacao($$, $1, $3, " >= ");
				
			}
			| E '<' E
			{
				realizaOperacao($$, $1, $3, " < ");
				
			}
			| E TK_LESS_EQUAL E
			{
				realizaOperacao($$, $1, $3, " <= ");
				
			}
			| E TK_EQUAL_EQUAL E
			{
				realizaOperacao($$, $1, $3, " == ");
				
			}
			| E TK_NOT_EQUAL E
			{
				realizaOperacao($$, $1, $3, " != ");
				
			}
			| E '%' E
			{
				//criando temporaria que recebera a soma
				SYMBOL_TYPE value;

				int caso = 0;
				
				if($1.tipo != "int"){
					yyerror("Não é possível realizar a operação  " + $1.label + " deve ser do tipo \"int\"\n");
				}else if($3.tipo != "int"){
					yyerror("Não é possível realizar a operação  " + $3.label + " deve ser do tipo \"int\"\n");
				} else{
					$$.label = geraIdAleatorio();	
					value.varName = $$.label;
					value.type =  "int";
					insereTempList(value.varName, value.type, tempList);
					$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
							$1.label + " % " + $3.label + ";\n";
				}
			}
			| E TK_AND E
			{
				realizaOperacao($$, $1, $3, " && ");
				
			}
			| E TK_OR E{
				realizaOperacao($$, $1, $3, " || ");
			}
			| TK_NOT E
			{
				//criando temporaria que recebera a soma
				SYMBOL_TYPE value;

				int caso = 0;

				//1° caso -> int e int					
				if($1.tipo == "int" && $3.tipo == "int"){
					$$.tipo = "int";
					value.type = "int";
					caso = 0;
				} 
				// 2° caso -> float e float
				if($1.tipo == "float" && $3.tipo == "float" ){
					$$.tipo = "float";
					value.type = "float";
					caso = 0;
				} 
				
				// 3° caso -> int e float
				if($1.tipo == "int" && $3.tipo == "float" ){
					$$.tipo = "float";
					value.type = "float";
					caso = 3;
				}
				// 4° caso -> float e int					
				if($1.tipo == "float" && $3.tipo == "int" ){
					$$.tipo = "float";
					value.type = "float";
					caso = 4;
				}
				
				//1° caso -> int e int		
				if(caso == 0){
					caso = 0;
					$$.label = geraIdAleatorio();	
					value.varName = $$.label;
					$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
							$1.label + operacao + $3.label + ";\n";	
				}

				// conversao int e float
				if(caso == 3){
					caso = 0;
					//Criando var de conversão implicita  e inserindo na lista de temps
					SYMBOL_TYPE tempConvert;
					tempConvert.temp = geraIdAleatorio();
					tempConvert.type = "float";
					insereTempList(tempConvert.temp, tempConvert.type, tempList);
					//Criando o label da var que vai receber a conversão
					$$.label = geraIdAleatorio();	
					value.varName = $$.label;
					
					$$.traducao = $1.traducao + $3.traducao + 
					"\t" + tempConvert.temp +  " = " + "(float)" + $1.label + ";\n"
					+ "\t" + $$.label + " = " +  $3.label + operacao + tempConvert.temp + ";\n";
				}
				// conversao float e int
				if(caso == 4){
					caso = 0;
					//Criando var de conversão implicita  e inserindo na lista de temps
					SYMBOL_TYPE tempConvert;
					tempConvert.temp = geraIdAleatorio();
					tempConvert.type = "float";
					insereTempList(tempConvert.temp, tempConvert.type, tempList);

					$$.label = geraIdAleatorio();	
					value.varName = $$.label;

					$$.traducao = $1.traducao + $3.traducao + 
					"\t" + tempConvert.temp +  " = " + "(float)" + $3.label + ";\n"
					+ "\t" + $$.label + " = " +  $1.label + operacao + tempConvert.temp + ";\n";
				}	


				value.temp = $$.label;				
				insereTempList(value.temp, value.type, tempList);
			}
			| TK_INT
			{
				$$.tipo = "int";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, tempList);
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";

			}
			| TK_FLOAT
			{
				$$.tipo = "float";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, tempList);
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";

			}
			| TK_TRUE
			{
				$$.tipo = "int";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, tempList);
				$$.traducao = "\t" + $$.label + " = " + "1" + ";\n";
			}
			| TK_FALSE
			{
				$$.tipo = "int";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, tempList);
				$$.traducao = "\t" + $$.label + " = " + "0" + ";\n";
			}
			| TK_ID
			{
				$$.traducao =  "";
				// cout << "aaaaa" << "\n";
				// $$.label =  geraIdAleatorio();
				// SYMBOL_TYPE value;
				// value.varName = $1.label;
				// value.temp = $$.label;
				
				// //insere id na tabela de simbolos
				// insertElement(SYMBOL_TABLE, $1.label, value);
				// //procurando elemento na hash
				// // SYMBOL_TYPE elemento = returnElement(SYMBOL_TABLE, $1.label);
				// // verificaDeclaracao($1.label, elemento.varName);
				// $$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";
			}
			;

TIPO 	: TK_TIPO_INT
		{
			$$.tipo = "int";
		}
		| TK_TIPO_FLOAT
		{
			$$.tipo = "float";
		}
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


	