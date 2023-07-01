%{
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <cstdlib>
#include <vector>
#include <stack>

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
	int isBool = 0;
};

typedef struct{
	string varName;
	string type;
	string temp;
} SYMBOL_TYPE;

//Criando prototipo da pilha de tabelas de símbolos
unordered_map<string, SYMBOL_TYPE> * criaTabela(){
    return new unordered_map<string, SYMBOL_TYPE>;
}

stack<unordered_map<string, SYMBOL_TYPE> *> pilha;

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
			return false;
        }else{
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

void verificaDeclaracao(unordered_map<string, SYMBOL_TYPE> hash, vector<SYMBOL_TYPE> list, string key){
	bool declarado = false;
	//key é label ou var1
	//verificar se a key é temporaria
	for(int i =0; i < list.size(); i++){
		if(list[i].varName == key){
			declarado = true;
		}
	}

	if(!declarado){
		if(hash.find(key) == hash.end()){
			yyerror("\'" + key + "\'" + " não foi declarado\n");
		}
	}
}

void verificaDeclaracaoPrevia(unordered_map<string, SYMBOL_TYPE> hash, string key){
	if(findElement(hash, key)){
		yyerror("\'" + key + "\'" + " já foi declarado\n");
	}
}

void realizaOperacao(atributos& $$, atributos& $1, atributos& $3, string operacao){
	if($1.isBool == 1 && $3.isBool != 1) yyerror("Operação inválida!\n" + $1.label + " é do tipo " + "bool" + " e " + $3.label + " é do tipo " + $3.tipo + "\n");
	if($1.isBool == 1 && $3.isBool == 1) yyerror("Operação inválida!\n" + $1.label + " é do tipo " + "bool" + " e " + $3.label + " é do tipo " + "bool" + "\n");
	if($1.isBool != 1 && $3.isBool == 1) yyerror("Operação inválida!\n" + $1.label + " é do tipo " + $1.tipo + " e " + $3.label + " é do tipo " + "bool" + "\n");
	if($1.tipo == "char" || $3.tipo == "char") yyerror("Operação inválida!\n" + $1.label + " é do tipo " + $1.tipo + " e " + $3.label + " é do tipo " + $3.tipo + "\n");
	
	bool S1Hash = findElement(SYMBOL_TABLE, $1.label);
	bool S3Hash = findElement(SYMBOL_TABLE, $3.label);
	//verificando se $1 e $3 estão na tabela de simbolos
	SYMBOL_TYPE elementS1 = returnElement(SYMBOL_TABLE, $1.label);					
	SYMBOL_TYPE elementS3 = returnElement(SYMBOL_TABLE, $3.label);					

	verificaDeclaracao(SYMBOL_TABLE, tempList, $1.label);
	verificaDeclaracao(SYMBOL_TABLE, tempList, $3.label);

	//criando temporaria que recebera a soma
	SYMBOL_TYPE value;

	int caso = 0;


	//1° caso -> int e int
	//Verificando se o elemento da possivel temporaria $3 é int 
	if($1.tipo == "int" && elementS3.type == "int"){
		$$.tipo = "int";
		value.type = "int";
		caso = 0;
	} 

	if(elementS1.type == "int" && $3.tipo == "int"){
		$$.tipo = "int";
		value.type = "int";
		caso = 0;
	} 

	if(elementS1.type == "int" && elementS3.type == "int"){
		$$.tipo = "int";
		value.type = "int";
		caso = 0;
	} 

	if($1.tipo == "int" && $3.tipo == "int"){
		$$.tipo = "int";
		value.type = "int";
		caso = 0;
	} 



	// 2° caso -> float e float
	if($1.tipo == "float" && elementS3.type == "float"){
		$$.tipo = "float";
		value.type = "float";
		caso = 0;
	} 

	if(elementS1.type == "float" && $3.tipo == "float"){
		$$.tipo = "float";
		value.type = "float";
		caso = 0;
	} 

	if(elementS1.type == "float" && elementS3.type == "float"){
		$$.tipo = "float";
		value.type = "float";
		caso = 0;
	} 

	if($1.tipo == "float" && $3.tipo == "float"){
		$$.tipo = "float";
		value.type = "float";
		caso = 0;
	} 

	

	// 3° caso -> int e float
	if($1.tipo == "int" && elementS3.type == "float"){
		$$.tipo = "float";
		value.type = "float";
		caso = 3;
	} 

	if(elementS1.type == "int" && $3.tipo == "float"){
		$$.tipo = "float";
		value.type = "float";
		caso = 3;
	} 

	if(elementS1.type == "int" && elementS3.type == "float"){
		$$.tipo = "float";
		value.type = "float";
		caso = 3;
	} 

	if($1.tipo == "int" && $3.tipo == "float"){
		$$.tipo = "float";
		value.type = "float";
		caso = 3;
	} 	


	// 4° caso -> float e int					
	if($1.tipo == "float" && elementS3.type == "int"){
		$$.tipo = "float";
		value.type = "float";
		caso = 4;
	} 

	if(elementS1.type == "float" && $3.tipo == "int"){
		$$.tipo = "float";
		value.type = "float";
		caso = 4;
	} 

	if(elementS1.type == "float" && elementS3.type == "int"){
		$$.tipo = "float";
		value.type = "float";
		caso = 4;
	} 

	if($1.tipo == "float" && $3.tipo == "int"){
		$$.tipo = "float";
		value.type = "float";
		caso = 4;
	} 	

	// 5° caso -> char e char					
	if($1.tipo == "char" && elementS3.type == "char"){
		$$.tipo = "char";
		value.type = "char";
		caso = 0;
	} 

	if(elementS1.type == "char" && $3.tipo == "char"){
		$$.tipo = "char";
		value.type = "char";
		caso = 0;
	} 

	if(elementS1.type == "char" && elementS3.type == "char"){
		$$.tipo = "char";
		value.type = "char";
		caso = 0;
	} 

	if($1.tipo == "char" && $3.tipo == "char"){
		$$.tipo = "char";
		value.type = "char";
		caso = 0;
	} 	
	
	//1° caso -> int e int		
	if(caso == 0){
		caso = 0;
		$$.label = geraIdAleatorio();	
		value.varName = $$.label;

		if(S1Hash == true && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
			SYMBOL_TABLE[$3.label].temp + operacao + SYMBOL_TABLE[$1.label].temp + ";\n";	
		}
		if(S1Hash == true && S3Hash == false){
			$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
			SYMBOL_TABLE[$1.label].temp  + operacao + $3.label + ";\n";	
		}
		if(S1Hash == false && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
			$1.label  + operacao + SYMBOL_TABLE[$3.label].temp + ";\n";	
		}
		if(S1Hash == false && S3Hash == false){
			$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
			$1.label  + operacao + $3.label + ";\n";	
		}
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

		if(S1Hash == true && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + SYMBOL_TABLE[$1.label].temp + ";\n"
			+ "\t" + $$.label + " = " +  SYMBOL_TABLE[$3.label].temp + operacao + tempConvert.temp + ";\n";
		}
		if(S1Hash == true && S3Hash == false){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + SYMBOL_TABLE[$1.label].temp + ";\n"
			+ "\t" + $$.label + " = " +  $3.label + operacao + tempConvert.temp + ";\n";
		}
		if(S1Hash == false && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + $1.label + ";\n"
			+ "\t" + $$.label + " = " +  SYMBOL_TABLE[$3.label].temp + operacao + tempConvert.temp + ";\n";	
		}
		if(S1Hash == false && S3Hash == false){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + $1.label + ";\n"
			+ "\t" + $$.label + " = " +  $3.label + operacao + tempConvert.temp + ";\n";	
		}
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

		if(S1Hash == true && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + SYMBOL_TABLE[$3.label].temp + ";\n"
			+ "\t" + $$.label + " = " +  SYMBOL_TABLE[$1.label].temp + operacao + tempConvert.temp + ";\n";	
		}
		if(S1Hash == true && S3Hash == false){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + $3.label + ";\n"
			+ "\t" + $$.label + " = " +  SYMBOL_TABLE[$1.label].temp + operacao + tempConvert.temp + ";\n";	
		}
		if(S1Hash == false && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + SYMBOL_TABLE[$3.label].temp + ";\n"
			+ "\t" + $$.label + " = " +  $1.label + operacao + tempConvert.temp + ";\n";	
		}
		if(S1Hash == false && S3Hash == false){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + $3.label + ";\n"
			+ "\t" + $$.label + " = " +  $1.label + operacao + tempConvert.temp + ";\n";
		}
	}	

	value.temp = $$.label;				
	insereTempList(value.temp, value.type, tempList);
}

void realizaOperacaoLogica(atributos& $$, atributos& $1, atributos& $3, string operacao){
	//Se o IsBool == 1, então é booleano
	if($1.isBool == 1 && $3.isBool != 1) yyerror("Operação inválida!\n" + $1.label + " é do tipo " + "bool" + " e " + $3.label + " é do tipo " + $3.tipo + "\n");
	if($1.isBool != 1 && $3.isBool == 1) yyerror("Operação inválida!\n" + $1.label + " é do tipo " + $1.tipo + " e " + $3.label + " é do tipo " + "bool" + "\n");
	if($1.tipo == "char" || $3.tipo == "char") yyerror("Operação inválida!\n" + $1.label + " é do tipo " + $1.tipo + " e " + $3.label + " é do tipo " + $3.tipo + "\n");
	
	realizaOperacao($$, $1, $3, operacao);
}

%}

%token TK_INT TK_FLOAT TK_CHAR TK_TIPO_BOOL TK_TRUE TK_FALSE
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_FLOAT TK_TIPO_CHAR
%token TK_CONVERT_FLOAT TK_CONVERT_INT
%token TK_FIM TK_ERROR
%token TK_GREATER_EQUAL TK_LESS_EQUAL TK_EQUAL_EQUAL TK_NOT_EQUAL
%token TK_AND TK_OR TK_NOT


%start S

%left '+' '-'
%left '*' '/'
%left TK_AND TK_OR
%left TK_LESS_EQUAL TK_GREATER_EQUAL
%left '>' '<' 
%nonassoc TK_CONVERT_FLOAT TK_CONVERT_INT

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
				//Ao abrir um novo bloco criamos uma nova tabela de simbolos e empilhamos
				pilha.push(criaTabela());
				unordered_map<string, SYMBOL_TYPE> * escopoAtual = pilha.top();
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
			| BLOCO
			| TK_TIPO_INT TK_ID ';'
			{
				verificaDeclaracaoPrevia(SYMBOL_TABLE, $2.label);
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
				verificaDeclaracaoPrevia(SYMBOL_TABLE, $2.label);
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
				verificaDeclaracaoPrevia(SYMBOL_TABLE, $2.label);
				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "int";
				value.temp = geraIdAleatorio();
				
				//insere id na tabela de simbolos
				insertElement(SYMBOL_TABLE, $2.label, value);
				insereTempList(value.temp, value.type, tempList);
				$$.traducao = $1.traducao + $2.traducao;
			}
			| TK_TIPO_CHAR TK_ID ';'
			{
				verificaDeclaracaoPrevia(SYMBOL_TABLE, $2.label);
				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "char";
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
			| TK_ID '=' TK_CHAR ';'
			{
				if(findElement(SYMBOL_TABLE, $1.label)){
					$$.traducao = $1.traducao + $3.traducao + "\t" + SYMBOL_TABLE[$1.label].temp + " = " +
														$3.label + ";\n";
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
			| TK_CONVERT_FLOAT E 
			{
				$$.tipo = "float";
				// cout << "\n\nTESTESS " + $$.tipo;  
				$$.label =  geraIdAleatorio();

				SYMBOL_TYPE tempConvert;
				tempConvert.temp = $$.label;
				tempConvert.type = $$.tipo;
				insereTempList(tempConvert.temp, tempConvert.type, tempList);

				$$.traducao = $1.traducao + $2.traducao + "\t" + tempConvert.temp + " = " + "(" + $$.tipo + ")--" + $2.label + ";\n";
			}
			| TK_CONVERT_INT E 
			{
				$$.tipo = "float";
				cout << "\n\nTESTESS " + $$.tipo;  
				$$.label =  geraIdAleatorio();

				SYMBOL_TYPE tempConvert;
				tempConvert.temp = $$.label;
				tempConvert.type = $$.tipo;
				insereTempList(tempConvert.temp, tempConvert.type, tempList);

				$$.traducao = $1.traducao + $2.traducao + "\t" + tempConvert.temp + " = " + "(" + $$.tipo + ")" + $2.label + ";\n";
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
				realizaOperacaoLogica($$, $1, $3, " && ");
			}
			| E TK_OR E{
				realizaOperacaoLogica($$, $1, $3, " || ");
			}
			| TK_NOT E
			{
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
				$$.isBool = 1;
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, tempList);
				$$.traducao = "\t" + $$.label + " = " + "1" + ";\n";
			}
			| TK_FALSE
			{
				$$.tipo = "int";
				$$.isBool = 1;
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, tempList);
				$$.traducao = "\t" + $$.label + " = " + "0" + ";\n";
			}
			| TK_CHAR
			{
				$$.tipo = "char";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, tempList);
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";	
					
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