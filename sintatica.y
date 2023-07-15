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
int labelBloco = 0;
int indiceEscopoAtual = 0;


struct atributos
{
	string label;
	string traducao;
	string tipo;
	//0 -> não é Booleano, 1 -> é boleano
	string isBool;
	char teste;
};

typedef struct{
	string varName;
	string type;
	string temp;
	string isBool;
	string value;
	string stringSize;
} SYMBOL_TYPE;

unordered_map<string, SYMBOL_TYPE> SYMBOL_TABLE;
vector<unordered_map<string, SYMBOL_TYPE>> pilha;
stack<string> pilhaLoop;

//vector para temporarias
vector<SYMBOL_TYPE> tempList;

int yylex(void);
void yyerror(string);

string geraIdAleatorio(){
	return "var" + to_string(contador++);
}

string geraLabelBloco(){
	return to_string(labelBloco++);
}

void empilha(vector<unordered_map<string, SYMBOL_TYPE>>& pilha){
    pilha.push_back(unordered_map<string, SYMBOL_TYPE>());
    indiceEscopoAtual = pilha.size() - 1;
}

void desempilha(vector<unordered_map<string, SYMBOL_TYPE>>& pilha){
    pilha.pop_back();
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

bool findElement(unordered_map<string, SYMBOL_TYPE> hash, string key){
	if(hash.find(key) == hash.end()){
		return false;
		
	}else{
		return true;
	}
}

int buscaEscopo(vector<unordered_map<string, SYMBOL_TYPE>>& pilha, string key){
	int indice = indiceEscopoAtual;

	while(indice >= 0){
		if(findElement(pilha[indice], key)) break;
		indice--;
	}
	//se não foi declarado e não for um 'number'
	return indice;
}

void atribuicao(atributos& $$, atributos& $1, atributos& $3, vector<unordered_map<string, SYMBOL_TYPE>>& pilha){
	//procurando o elemento nos varios escopo
	int aux = buscaEscopo(pilha, $1.label);
	
	if(aux < 0 ){
		yyerror("ERROR:" + $1.label + " não foi declarado.");
	}
	
	// if($3.label == "true") $$.traducao = $1.traducao + $3.traducao + "\t" + pilha[aux][$1.label].temp + " = " + "true" + ";\n";
	pilha[aux][$1.label].value = $3.label;
	if(pilha[aux][$1.label].type != "string"){
		$$.traducao = $1.traducao + $3.traducao + "\t" + pilha[aux][$1.label].temp + " = " + $3.label + ";\n";	
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
	for(int i = 0; i < list.size(); i++){
		// cout << list[i].varName + "\n";
		if(list[i].type == "string" && list[i].stringSize == "0") cout << "\t" << "char *" << " " << list[i].varName << ";\n";
		if(list[i].type == "string" && list[i].stringSize != "0") cout << "\t" << "char" << " " << list[i].varName << "[" << list[i].stringSize << "]" << ";\n";
		if(list[i].type != "string")cout << "\t" << list[i].type << " " << list[i].varName << ";\n";
	}
} 

void insereTempList(string label, string tipo, int tamanho, vector<SYMBOL_TYPE>& tempList){
	SYMBOL_TYPE temp;
	temp.varName = label;
	temp.type = tipo;
	temp.stringSize = to_string(tamanho);
	tempList.push_back(temp);
}

void verificaDeclaracao(unordered_map<string, SYMBOL_TYPE> hash, vector<SYMBOL_TYPE> list, string key){
	//key => '1'
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

bool procuraNaListaTemp(vector<SYMBOL_TYPE> list, string key){
	//key => '1'
	bool declarado = false;
	//key é label ou var1
	//verificar se a key é temporaria
	for(int i =0; i < list.size(); i++){
		if(list[i].varName == key){
			declarado = true;
			break;
		}
	}	
	return declarado;
}

SYMBOL_TYPE retornaListaTemp(vector<SYMBOL_TYPE> list, string key){
	SYMBOL_TYPE element;
	for(int i =0; i < list.size(); i++){
		if(list[i].varName == key){
			element = list[i];
			break;
		}
	}	
	return element;
}


void verificaDeclaracaoPrevia(unordered_map<string, SYMBOL_TYPE> hash, string key){
	if(findElement(hash, key)){
		yyerror("\'" + key + "\'" + " já foi declarado\n");
	}
}

void realizaOperacao(atributos& $$, atributos& $1, atributos& $3, string operacao){

	bool S1Hash;
	bool S3Hash;
	SYMBOL_TYPE elementS1;
	SYMBOL_TYPE elementS3;

	//verifica se é um id ou número na Tabela de Simbolos, true se estiver, false se não estiver

	int indiceS1 = buscaEscopo(pilha, $1.label);
	//true se esta na temp, false se nao esta
	int S1estaNaTemp = procuraNaListaTemp(tempList, $1.label);
	
	//Se o indice < 0, não está na lista de temps, é uma var não declarada
	if(indiceS1 < 0 && !(S1estaNaTemp)){
		//erro
		yyerror("ERRO!" + $1.label + "não foi declarada.");
	}
	//Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
	if(S1estaNaTemp){
		
		S1Hash = false;
	}

	if(indiceS1 >= 0){
		S1Hash = true;
		elementS1 = returnElement(pilha[indiceS1], $1.label);
	}


	int indiceS3 =  buscaEscopo(pilha, $3.label);
	int S3estaNaTemp = procuraNaListaTemp(tempList, $3.label);

	if(indiceS3 < 0 && !(S3estaNaTemp)){
		//erro
		yyerror("erro");
	}
	//Caso onde o elemento S1 é um 'number', 
	if(S3estaNaTemp){
		S3Hash = false;
	}
	if(indiceS3 >= 0){
		S3Hash = true;
		elementS3 = returnElement(pilha[indiceS3], $3.label);
	}

	cout << elementS1.isBool + "\n";
	//ID(bool) e QUALQUER COISA
	if(elementS1.isBool == "bool" && $3.isBool != "bool") yyerror("Operação inválida!\n" + $1.label + " é do tipo " + "bool" + " e " + $3.label + " é do tipo " + $3.tipo + "\n");
	//QUALQUER COISA e ID(bool) 
	if($1.isBool != "bool" && elementS3.isBool == "bool") yyerror("Operação inválida!\n" + $1.label + " é do tipo " + $1.tipo + " e " + $3.label + " é do tipo " + "bool" + "\n");
	//ID(bool) e ID(não é booleano)
	if(elementS1.isBool == "bool" && elementS3.isBool != "bool") yyerror("Operação inválida!\n" + $1.label + " é do tipo " + "bool" + " e " + $3.label + " é do tipo " + elementS3.type + "\n");
	//ID(não é booleano) e ID(bool)
	if(elementS1.isBool != "bool" && elementS3.isBool == "bool") yyerror("Operação inválida!\n" + $1.label + " é do tipo " + elementS3.type + " e " + $3.label + " é do tipo " + "bool" + "\n");
	

	// if($1.isBool != 1 && $3.isBool == 1) yyerror("Operação inválida!\n" + $1.label + " é do tipo " + $1.tipo + " e " + $3.label + " é do tipo " + "bool" + "\n");
	if($1.tipo == "char" || $3.tipo == "char") yyerror("Operação inválida!\n" + $1.label + " é do tipo " + $1.tipo + " e " + $3.label + " é do tipo " + $3.tipo + "\n");
		
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
	
	//aqui
	//Se o operador for logico, ou relacional, mudamos o tipo da variavel auxiliar para armazenar ele 
	if(operacao == " > " || operacao == " < " || operacao == " >= " || operacao == " =< " 
	|| operacao == " || " || operacao == " && " || operacao == " != " || operacao == " == "){
		//verificar o 
	}
	
	//1° caso -> int e int		
	if(caso == 0){
		
		caso = 0;
		$$.label = geraIdAleatorio();	
		value.varName = $$.label;
		// //Se o operador for logico, ou relacional, mudamos o tipo da variavel auxiliar para armazenar ele 
		if(operacao == " > " || operacao == " < " || operacao == " >= " || operacao == " =< " 
		|| operacao == " || " || operacao == " && " || operacao == " != " || operacao == " == "){
			$$.isBool = "bool";
		}
		//Quando é um ID operacao com ID
		if(S1Hash == true && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
			pilha[indiceS3][$3.label].temp + operacao + pilha[indiceS1][$1.label].temp + ";\n";	
		}
		//Quando é um ID operacao com um temp(1, 2, ... 999999)
		if(S1Hash == true && S3Hash == false){
			$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
			pilha[indiceS1][$1.label].temp  + operacao + $3.label + ";\n";	
		}
		//Quando é um temp(1, 2, ... 999999) operacao ID
		if(S1Hash == false && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
			$1.label  + operacao + pilha[indiceS3][$3.label].temp + ";\n";	
		}
		//Quando é um temp(1, 2, ... 999999) operacao temp(1, 2, ... 999999)
		
		if(S1Hash == false && S3Hash == false){
			// cout << "adopkaplasd\n";	
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
		insereTempList(tempConvert.temp, tempConvert.type, 0, tempList);
		//Criando o label da var que vai receber a conversão
		$$.label = geraIdAleatorio();	
		value.varName = $$.label;


		//Quando é um ID operacao com ID
		// if(S1Hash == true && S3Hash == true){
		// 	$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
		// 	pilha[indiceS3][$3.label].temp + operacao + pilha[indiceS1][$1.label].temp + ";\n";	
		// }


		if(S1Hash == true && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + pilha[indiceS1][$1.label].temp + ";\n"
			+ "\t" + $$.label + " = " +  pilha[indiceS3][$3.label].temp + operacao + tempConvert.temp + ";\n";
		}
		if(S1Hash == true && S3Hash == false){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + pilha[indiceS1][$1.label].temp + ";\n"
			+ "\t" + $$.label + " = " +  $3.label + operacao + tempConvert.temp + ";\n";
		}
		if(S1Hash == false && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + $1.label + ";\n"
			+ "\t" + $$.label + " = " +  pilha[indiceS3][$3.label].temp + operacao + tempConvert.temp + ";\n";	
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
		insereTempList(tempConvert.temp, tempConvert.type, 0, tempList);

		$$.label = geraIdAleatorio();	
		value.varName = $$.label;

		if(S1Hash == true && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + pilha[indiceS3][$3.label].temp + ";\n"
			+ "\t" + $$.label + " = " +  pilha[indiceS1][$1.label].temp + operacao + tempConvert.temp + ";\n";	
		}
		if(S1Hash == true && S3Hash == false){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + $3.label + ";\n"
			+ "\t" + $$.label + " = " +  pilha[indiceS1][$1.label].temp + operacao + tempConvert.temp + ";\n";	
		}
		if(S1Hash == false && S3Hash == true){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + pilha[indiceS3][$3.label].temp + ";\n"
			+ "\t" + $$.label + " = " +  $1.label + operacao + tempConvert.temp + ";\n";	
		}
		if(S1Hash == false && S3Hash == false){
			$$.traducao = $1.traducao + $3.traducao + 
			"\t" + tempConvert.temp +  " = " + "(float)" + $3.label + ";\n"
			+ "\t" + $$.label + " = " +  $1.label + operacao + tempConvert.temp + ";\n";
		}
	}	
	
	value.temp = $$.label;				
	insereTempList(value.temp, value.type, 0, tempList);	
}

void tkIDAtribuicao(atributos& $$, atributos& $1, atributos& $3, vector<unordered_map<string, SYMBOL_TYPE>>& pilha){
	//o tipo do tk id e do E tem q ser verificado
	bool S1IsId;
	bool S3IsId;
	SYMBOL_TYPE elementS1;
	SYMBOL_TYPE elementS3;
	
	// verifica se é um id ou número na Tabela de Simbolos, true se estiver, false se não estiver
	int indiceS1 = buscaEscopo(pilha, $1.label);
	//true se esta na temp, false se nao esta
	int S1estaNaTemp = procuraNaListaTemp(tempList, $1.label);
	
	// //Se o indice < 0, não está na lista de temps, é uma var não declarada
	if(indiceS1 < 0 && !(S1estaNaTemp)){
		//erro
		yyerror("ERRO!" + $1.label + "não foi declarada.");
	}
	// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
	if(S1estaNaTemp){
		S1IsId = false;
	}
	
	if(indiceS1 >= 0){
		S1IsId = true;
		elementS1 = returnElement(pilha[indiceS1], $1.label);
	}

	int indiceS3 =  buscaEscopo(pilha, $3.label);
	int S3estaNaTemp = procuraNaListaTemp(tempList, $3.label);

	if(indiceS3 < 0 && !(S3estaNaTemp)){
		//erro
		yyerror("erro");
	}
	//Caso onde o elemento S1 é um 'number', 
	if(S3estaNaTemp){
		S3IsId = false;
	}
	if(indiceS3 >= 0){
		S3IsId = true;
		elementS3 = returnElement(pilha[indiceS3], $3.label);
	}
	
	// SYMBOL_TYPE value = returnElement(pilha[indiceS1], $1.label);
	// Se o tipo do TK_ID for igual ao tipo da Expressão, não alteramos o tipo da Expressão, atribuindo normalmente
	
	if(S1IsId == true && S3IsId == true){
		if(elementS1.isBool != "bool" && elementS3.isBool == "bool") yyerror("Atribuição inválida!\n");
		//Caso de ID e ID 
		if((elementS1.type == pilha[indiceS3][$3.label].type)){
			$$.traducao = $1.traducao + $3.traducao + "\t" + pilha[indiceS1][$1.label].temp + " = " 
			+ pilha[indiceS3][$3.label].temp + ";\n";
		}
		//Caso de ID e ID com tipos diferentes, ou seja, ocorre uma conversão
		if((elementS1.type != pilha[indiceS3][$3.label].type)){
			$$.traducao = $1.traducao + $3.traducao + "\t" + pilha[indiceS1][$1.label].temp + " = " + 
			"(" + elementS1.type + ")" + pilha[indiceS3][$3.label].temp + ";\n";		
		}

		if(pilha[indiceS1][$1.label].type == "string"){
			cout << "BBBBBB\n";
			$$.traducao = $1.traducao + $3.traducao + "\t" + 
			pilha[indiceS1][$1.label].temp + " = " + "(char *) malloc(" + pilha[indiceS3][$3.label].stringSize + ")" + ";\n" +
			"\t" + "strcpy(" + pilha[indiceS1][$1.label].temp + ", " + pilha[indiceS3][$3.label].temp + ")"  + ";\n";
		}
	}
	//Caso de ID e TEMP
	if(S1IsId == true && S3IsId == false){
		if(elementS1.isBool != "bool" && $3.isBool == "bool") yyerror("Atribuição inválida!\n");

		//precisamos fazer isso para verificar o tamanho da string
		elementS3 = retornaListaTemp(tempList, $3.label);	

		//Caso de ID e TEMP
		if((elementS1.type == $3.tipo)){
			$$.traducao += $1.traducao + $3.traducao + "\t" + pilha[indiceS1][$1.label].temp + " = " 
			+ $3.label + ";\n";
		}
		//Caso de ID e TEMP com tipos diferetes, fazendo conversao 
		if((elementS1.type != $3.tipo)){
			$$.traducao = $1.traducao + $3.traducao + "\t" + pilha[indiceS1][$1.label].temp + " = " + 
			"(" + elementS1.type + ")" + $3.label + ";\n";			
		}

		if(pilha[indiceS1][$1.label].type == "string"){
			pilha[indiceS1][$1.label].stringSize = elementS3.stringSize;
			$$.traducao = $1.traducao + $3.traducao + "\t" + 
			pilha[indiceS1][$1.label].temp + " = " + "(char *) malloc(" + elementS3.stringSize + ")" + ";\n" +
			"\t" + string("strcpy(") + pilha[indiceS1][$1.label].temp + ", " + $3.label + ")"  + ";\n";
		}
	}		
}

void verificaExpressao(atributos& $, bool& isID, bool& isInTemp, SYMBOL_TYPE& element, int& indicePilha,
						vector<unordered_map<string, SYMBOL_TYPE>>& pilha, unordered_map<string, SYMBOL_TYPE>& hash){
	indicePilha = buscaEscopo(pilha, $.label);
	//true se esta na temp, false se nao esta
	isInTemp = procuraNaListaTemp(tempList, $.label);
	
	// //Se o indice < 0, não está na lista de temps, é uma var não declarada
	if(indicePilha < 0 && !(isInTemp)){
		//erro
		yyerror("ERRO!" + $.label + "não foi declarada.");
	}
	// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
	if(isInTemp){
		isID = false;
	}
	
	if(indicePilha >= 0){
		isID = true;
		element = returnElement(pilha[indicePilha], $.label);
	}
}

%}

%token TK_INT TK_FLOAT TK_CHAR TK_TIPO_BOOL TK_TRUE TK_FALSE TK_STRING
%token TK_MAIN TK_ID TK_TIPO_INT TK_TIPO_FLOAT TK_TIPO_CHAR TK_TIPO_STRING
%token TK_CONVERT_FLOAT TK_CONVERT_INT
%token TK_FIM TK_ERROR
%token TK_GREATER_EQUAL TK_LESS_EQUAL TK_EQUAL_EQUAL TK_NOT_EQUAL
%token TK_AND TK_OR TK_NOT
%token TK_VIRGULA TK_PRINT
%token TK_IF TK_ELSE
%token TK_DO TK_WHILE TK_FOR TK_BREAK TK_SWITCH TK_CASE TK_DEFAULT TK_DP



%start S

%left TK_AND TK_OR
%left TK_LESS_EQUAL TK_GREATER_EQUAL
%left '>' '<'
%left '+' '-'
%left '*' '/' 
%nonassoc TK_CONVERT_FLOAT TK_CONVERT_INT

%%

S 			: TK_TIPO_INT TK_MAIN '(' ')' BLOCO
			{
				cout << "/*Compilador FOCA*/\n" << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\nint main(void)\n{\n";
				printList(tempList); 
				cout << $5.traducao << "\treturn 0;\n}" << endl; 
			}
			;

BLOCO       : '{' BLOCO_INICIO COMANDOS BLOCO_FIM '}'
            {
                $$.traducao = $3.traducao;
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


BLOCO_INICIO:
            {
                empilha(pilha);
            }
            ;

BLOCO_FIM   :
            { 
                desempilha(pilha);
            }
            ;

/* BLOCO_LOOP  : '{' BLOCO_INICIO COMANDOS BLOCO_FIM '}'
            {
                $$.traducao = $3.traducao;
            }
			| '{' BLOCO_INICIO COMANDOS TK_BREAK
            ;

			
COMANDOS_LOOP: COMANDO COMANDOS
			{
				$$.traducao =  $1.traducao + $2.traducao;
			}
			|
			{	
				$$.traducao = "";
			}
			; */

BLOCO_SWITCH: '{' CASE_LISTA '}'
            {
                // Faça o processamento necessário para o bloco switch
                $$.traducao += $2.traducao;
            }
            ;

CASE_LISTA  : CASE_STMT CASE_LISTA
            {
                $$.traducao += $2.traducao;
            }
            |
            {
                // Caso base: não há mais casos na lista
                $$.traducao = "";
            }
            ;


CASE_STMT   : CASE
            {
                // Faça o processamento necessário para o caso
            }
            ;

CASE        : TK_CASE E TK_DP COMANDOS
            {
				bool S2isId;
				SYMBOL_TYPE elementS2;
				int indiceS2;
				bool S2estaNaTemp;
				
				verificaExpressao($2, S2isId, S2estaNaTemp, elementS2, indiceS2, pilha, SYMBOL_TABLE);

				if(S2isId){
					string idSwitch = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS2.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					$$.traducao = $2.traducao + "\t" + "INICIO_CASE" + idSwitch + ":\n" +
					"\t" + value.temp + " = !" + elementS2.temp + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_CASE" + idSwitch + ";\n" +
					$4.traducao +
					"\t" + "FIM_CASE" + idSwitch + ":\n";
				}
				if(!S2isId){
					string idSwitch = geraLabelBloco();	
					SYMBOL_TYPE value;
					value.type = $2.tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);
					pilhaLoop.push(value.temp);

					$$.traducao = $2.traducao + "\t" + "INICIO_CASE" + idSwitch + ":\n" +
					"\t" + value.temp + " = !" + $2.label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_CASE" + idSwitch + ";\n" +
					$4.traducao +
					"\t" + "FIM_CASE" + idSwitch + ":\n";
				}





                // if(verificaVar($2.label)){
                //     if(pilha[busca_escopo($2.label)][$2.label].atribuido == 0){
                //        yyerror("ERRO: Variável " + $2.label + " sem valor atribuido");
                //     }
                //     else{
                //         $$.declaracao += $2.declaracao + $4.declaracao;
                //         string rotuloIni = genRot();
                //         string rotuloFim = genRot();
                //         $$.label = genTemp();
                //         $$.declaracao += "\t" + string("int ") + $$.label + ";\n";
                //         $$.traducao += "\t" + rotuloIni + "\n"; 
                //         $$.traducao += $2.traducao;
                //         $$.traducao += "\t" + $$.label + " = " + "!" + pilha[busca_escopo($2.label)][$2.label].temp + ";\n";
                //         $$.traducao += "\tif("+ $$.label + ")" + " goto " + rotuloFim + ";\n";
                //         $$.traducao += $4.traducao;
                //         $$.traducao += "\t"+ rotuloFim +"\n";
                //     }
                // }
                // else{
                //         $$.declaracao += $2.declaracao + $4.declaracao;
                //         string rotuloIni = genRot();
                //         string rotuloFim = genRot();
                //         $$.label = genTemp();
                //         $$.declaracao += "\t" + string("int ") + $$.label + ";\n";
                //         $$.traducao += "\t" + rotuloIni + "\n"; 
                //         $$.traducao += $2.traducao;
                //         $$.traducao += "\t" + $$.label + " = " + "!" + $2.label + ";\n";
                //         $$.traducao += "\tif("+ $$.label + ")" + " goto " + rotuloFim + ";\n";
                //         $$.traducao += $4.traducao;
                //         $$.traducao += "\t"+ rotuloFim +"\n";
                // }
                

            }
            | TK_DEFAULT TK_DP COMANDOS
            {
                // if(defaultExecutado){
                //    yyerror("ERRO: Mais de um case padrão (default) encontrado.");
                // }else{
                //     $$.declaracao += $3.declaracao;
                //     string rotuloIni = genRot();
                //     string rotuloFim = genRot();
                //     $$.traducao += "\t" + rotuloIni + "\n"; 
                //     $$.traducao += $3.traducao;
                //     $$.traducao += "\t"+ rotuloFim +"\n";
                //     defaultExecutado = true;
                // }
            }
            ;



COMANDO 	: E ';'
			| BLOCO
			| TK_TIPO_INT TK_ID ';'
			{
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], $2.label);
				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "int";
				value.temp = geraIdAleatorio();
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], $2.label, value);
				insereTempList(value.temp, value.type, 0, tempList);
				$$.traducao = $1.traducao + $2.traducao;
			}
			| TK_TIPO_FLOAT TK_ID ';'
			{
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], $2.label);
				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "float";
				value.temp = geraIdAleatorio();
				
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], $2.label, value);
				insereTempList(value.temp, value.type, 0, tempList);
				$$.traducao = $1.traducao + $2.traducao;
			}
			| TK_TIPO_BOOL TK_ID ';'
			{
				// $$.teste = 'Z';
				// $2.label = "pastel de frango 2 reais";
				// $2.isBool = 1;
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], $2.label);
				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "int";
				value.isBool = "bool";
				value.temp = geraIdAleatorio();
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], $2.label, value);
				insereTempList(value.temp, value.type, 0, tempList);
				
				$$.traducao = $1.traducao + $2.traducao;
			}
			| TK_TIPO_CHAR TK_ID ';'
			{
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], $2.label);
				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "char";
				value.temp = geraIdAleatorio();				
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], $2.label, value);
				insereTempList(value.temp, value.type, 0, tempList);
				$$.traducao = $1.traducao + $2.traducao;
			}
			| TK_TIPO_STRING TK_ID ';'
			{
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], $2.label);
				SYMBOL_TYPE value;
				value.varName = $2.label;
				value.type = "string";
				value.stringSize = "0";
				value.temp = geraIdAleatorio();				
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], $2.label, value);
				insereTempList(value.temp, value.type, 0, tempList);
				$$.traducao = $1.traducao + $2.traducao;
			}
			| TK_ID '=' TK_INT ';'
			{
				atribuicao($$, $1, $3, pilha);
			}		
			| TK_ID '=' TK_FLOAT ';'
			{
				atribuicao($$, $1, $3, pilha);
			}
			| TK_ID '=' TK_TRUE ';'
			{
				string valor = "1";
				//procurando o elemento nos varios escopo
				int aux = buscaEscopo(pilha, $1.label);
				
				if(aux < 0 ){
					yyerror("ERROR:" + $1.label + " não foi declarado.");
				}
				pilha[aux][$1.label].isBool = "bool";
				$$.traducao = $1.traducao + $3.traducao + "\t" + pilha[aux][$1.label].temp + " = " + valor + ";\n";
			}			
			| TK_ID '=' TK_FALSE ';'
			{
				$3.label = "0";
				atribuicao($$, $1, $3, pilha);
			}
			| TK_ID '=' TK_CHAR ';'
			{
				atribuicao($$, $1, $3, pilha);
			}
			/* | TK_ID '=' TK_STRING ';'
			{
				atribuicao($$, $1, $3, pilha);
			}			 */
			| TK_ID '=' E ';'
			{	
				tkIDAtribuicao($$, $1, $3, pilha);
			}
			| TK_PRINT '(' E COISAS ')' ';'
			{
				// std::cout << "Exemplo de impressão literal de \\n: \\n\n";
				
				// cout << $3.label + "--------------";
				// if($3.label == "_p") $$.traducao = $3.traducao + "\t" + "cout" + " << " + "endl" + ";\n";

				bool S1IsId;
				bool S3IsId;
				SYMBOL_TYPE elementS1;
				SYMBOL_TYPE elementS3;
				
				// verifica se é um id ou número na Tabela de Simbolos, true se estiver, false se não estiver
				int indiceS1 = buscaEscopo(pilha, $3.label);
				//true se esta na temp, false se nao esta
				int S1estaNaTemp = procuraNaListaTemp(tempList, $3.label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS1 < 0 && !(S1estaNaTemp)){
					//erro
					yyerror("ERRO!" + $3.label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S1estaNaTemp){
					S1IsId = false;
				}
				
				if(indiceS1 >= 0){
					S1IsId = true;
					elementS1 = returnElement(pilha[indiceS1], $3.label);
				}

				//Quando o COISAS TA VAZIO 
				if($4.label == "") $$.traducao = $3.traducao + $4.traducao + "\t" + "cout" + " << " + elementS1.temp + ";\n";

				if($4.label != ""){
					//pegar o que tem no coisas, guardar numa variavel e depois printar
					$$.traducao = $3.traducao + $4.traducao + "\t" + "cout" + " << " + elementS1.temp + ";\n";	
				} 	
			}
			| TK_IF '(' E ')' BLOCO
			{
				bool S3IsId;
				SYMBOL_TYPE elementS3;

				int indiceS3 = buscaEscopo(pilha, $3.label);
				//true se esta na temp, false se nao esta
				bool S3estaNaTemp = procuraNaListaTemp(tempList, $3.label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS3 < 0 && !(S3estaNaTemp)){
					//erro
					yyerror("ERRO!" + $3.label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S3estaNaTemp){
					S3IsId = false;
				}
				
				if(indiceS3 >= 0){
					S3IsId = true;
					elementS3 = returnElement(pilha[indiceS3], $3.label);
				}

				if(S3IsId){
					if(elementS3.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idBloco = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS3.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					$$.traducao = $3.traducao + "\t" + value.temp + " = !" + elementS3.temp + ";\n" +
					"\t" + "if(" + elementS3.temp + ") " + "goto " + "FIM_IF" + idBloco + ";\n" +
					$5.traducao +
					"\t" + "FIM_IF" + idBloco + ":\n";
				}

				if(!S3IsId){
					if($3.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idBloco = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = $3.tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					$$.traducao = $3.traducao + "\t" + value.temp + " = !" + $3.label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_IF" + idBloco + ";\n" +
					$5.traducao +
					"\t" + "FIM_IF" + idBloco + ":\n";
				}
			}
			| TK_IF '(' E ')' BLOCO TK_ELSE BLOCO
			{
				bool S3IsId;
				SYMBOL_TYPE elementS3;

				int indiceS3 = buscaEscopo(pilha, $3.label);
				//true se esta na temp, false se nao esta
				bool S3estaNaTemp = procuraNaListaTemp(tempList, $3.label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS3 < 0 && !(S3estaNaTemp)){
					//erro
					yyerror("ERRO!" + $3.label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S3estaNaTemp){
					S3IsId = false;
				}
				
				if(indiceS3 >= 0){
					S3IsId = true;
					elementS3 = returnElement(pilha[indiceS3], $3.label);
				}

				if(S3IsId){
					if(elementS3.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idBloco = geraLabelBloco();
					string idElse = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS3.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					$$.traducao = $3.traducao + "\t" + value.temp + " = !" + elementS3.temp + ";\n" +
					"\t" + "if(" + elementS3.temp + ") " + "goto " + "INICIO_ELSE" + idElse + ";\n" +
					$5.traducao +
					"\t" + "goto " + "FIM_IF" + idBloco + ";\n" +
					"\t" + "INICIO_ELSE" + idElse + ":\n" +
					$7.traducao +
					"\t" + "FIM_IF" + idBloco + ":\n";
				}

				if(!S3IsId){
					if($3.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idBloco = geraLabelBloco();
					string idElse = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = $3.tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					$$.traducao = $3.traducao + "\t" + value.temp + " = !" + $3.label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "INICIO_ELSE" + idElse + ";\n" +
					$5.traducao +
					"\t" + "goto " + "FIM_IF" + idBloco + ";\n" +
					"\t" + "INICIO_ELSE" + idElse + ":\n" +
					$7.traducao +
					"\t" + "FIM_IF" + idBloco + ":\n";
				}
			}
			| TK_WHILE '(' E ')' BLOCO
			{
				bool S3IsId;
				SYMBOL_TYPE elementS3;
				int indiceS3;
				bool S3estaNaTemp;
				// void verificaExpressao(atributos& $, bool& isID, bool& isInTemp, SYMBOL_TYPE& element, int& indicePilha,
				// 		vector<unordered_map<string, SYMBOL_TYPE>>& pilha, unordered_map<string, SYMBOL_TYPE>& hash)

				verificaExpressao($3, S3IsId, S3estaNaTemp, elementS3, indiceS3, pilha, SYMBOL_TABLE);

				if(S3IsId){
					if(elementS3.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS3.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					$$.traducao = $3.traducao + "\t" + "INICIO_WHILE" + idWhile + ":\n" +
					"\t" + value.temp + " = !" + elementS3.temp + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_WHILE" + idWhile + ";\n" +
					$5.traducao +
					"\t" + "goto " + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + "FIM_WHILE" + idWhile + ":\n";
				}
				if(!S3IsId){
					if($3.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = $3.tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					$$.traducao = $3.traducao + "\t" + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + value.temp + " = !" + $3.label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_WHILE" + idWhile + ";\n" +
					$5.traducao +
					"\t" + "goto " + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + "FIM_WHILE" + idWhile + ":\n"; 
				}
			}
			| TK_DO BLOCO TK_WHILE '(' E ')' ';'
			{
				bool S5IsId;
				SYMBOL_TYPE elementS5;

				int indiceS5 = buscaEscopo(pilha, $5.label);
				//true se esta na temp, false se nao esta
				bool S5estaNaTemp = procuraNaListaTemp(tempList, $5.label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS5 < 0 && !(S5estaNaTemp)){
					//erro
					yyerror("ERRO!" + $5.label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S5estaNaTemp){
					S5IsId = false;
				}
				
				if(indiceS5 >= 0){
					S5IsId = true;
					elementS5 = returnElement(pilha[indiceS5], $5.label);
				}

				if(S5IsId){
					if(elementS5.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS5.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					$$.traducao = "\tINICIO_DOWHILE" + idWhile + ";\n" +
					$2.traducao +
					"\t" + value.temp + " = !" + elementS5.temp + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_DOWHILE" + idWhile + ";\n" +
					"\t" + "goto " + "INICIO_DOWHILE" + idWhile + ";\n" +
					"\t" + "FIM_DOWHILE" + idWhile + ":\n"; 
				}

				if(!S5IsId){
					if($5.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = $3.tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					$$.traducao = $2.traducao + "\t" + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + value.temp + " = !" + $5.label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_WHILE" + idWhile + ";\n" +
					$2.traducao +
					"\t" + "goto " + "INICIO_WHILE" + idWhile + ";\n";
				}
			}					
			| TK_FOR '(' TK_ID '=' E ';' E ';' TK_ID '=' E ')' BLOCO
			{
				bool S7isId;
				SYMBOL_TYPE elementS7;

				int indiceS7 = buscaEscopo(pilha, $7.label);
				//true se esta na temp, false se nao esta
				bool S7EstaNaTemp = procuraNaListaTemp(tempList, $7.label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS7 < 0 && !(S7EstaNaTemp)){
					//erro
					yyerror("ERRO!" + $7.label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S7EstaNaTemp){
					S7isId = false;
				}
				
				if(indiceS7 >= 0){
					S7isId = true;
					elementS7 = returnElement(pilha[indiceS7], $7.label);
				}
				cout << "FUNCAO\n";
				
				if(!S7isId){
					if($7.isBool != "bool") yyerror("---A expressão não é do tipo booleano!\n");
					string idFor = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = $7.tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);
					tkIDAtribuicao($$, $3, $5, pilha);
					$$.traducao +=  $3.traducao + $7.traducao + "\t" + "INICIO_FOR" + idFor + ":\n" +
					"\t" + value.temp + " = !" + $7.label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_FOR" + idFor + ";\n" +
					$13.traducao;
					tkIDAtribuicao($$, $3, $11, pilha); 
					$$.traducao += "\tgoto INICIO_FOR" + idFor + ";\n" +
					"\t" + "FIM_FOR" + idFor + ":\n"; 
				}
			}
			| TK_SWITCH '(' E ')' BLOCO_SWITCH
			{
				bool S3isId;
				SYMBOL_TYPE elementS3;
				int indiceS3;
				bool S3estaNaTemp;
				
				verificaExpressao($3, S3isId, S3estaNaTemp, elementS3, indiceS3, pilha, SYMBOL_TABLE);
				string valueTemp = pilhaLoop.top();

				$$.traducao = $3.traducao + $5.traducao;
			}
			/* | CASE 
			{
				
			}
			| TK_BREAK ';'
			{
				cout << "aaaaaaa\n";
				string label;
				label = pilhaLoop.top();
				cout << "aaaaaaa\n";
				$$.traducao = "\tgoto " + label + ";\n";
				pilhaLoop.pop();
			} */
			;

COISAS		: TK_VIRGULA E COISAS
			{	
				bool S1IsId;
				bool S3IsId;
				
				SYMBOL_TYPE element;
				// verifica se é um id ou número na Tabela de Simbolos, true se estiver, false se não estiver
				int indiceS1 = buscaEscopo(pilha, $2.label);
				//true se esta na temp, false se nao esta
				int S1estaNaTemp = procuraNaListaTemp(tempList, $2.label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS1 < 0 && !(S1estaNaTemp)){
					//erro
					yyerror("ERRO!" + $3.label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S1estaNaTemp){
					S1IsId = false;
				}
				
				if(indiceS1 >= 0){
					S1IsId = true;
					element = returnElement(pilha[indiceS1], $2.label);
				}
		
				if(element.type == "int"){
					string valor = element.value;
					int stringSize = valor.size();
					// $$.traducao = $2.traducao + $3.traducao + "\t" + "cout" + " << " + elementS1.temp + ";\n";
				}
				$$.traducao = $2.traducao + $3.traducao;
			}
			|
			{
				$$.traducao = "";
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
				$$.label =  geraIdAleatorio();

				SYMBOL_TYPE tempConvert;
				tempConvert.temp = $$.label;
				tempConvert.type = $$.tipo;
				insereTempList(tempConvert.temp, tempConvert.type, 0, tempList);

				$$.traducao = $1.traducao + $2.traducao + "\t" + tempConvert.temp + " = " + "(" + $$.tipo + ")" + $2.label + ";\n";
			}
			| TK_CONVERT_INT E 
			{
				$$.tipo = "float";
				$$.label =  geraIdAleatorio();

				SYMBOL_TYPE tempConvert;
				tempConvert.temp = $$.label;
				tempConvert.type = $$.tipo;
				insereTempList(tempConvert.temp, tempConvert.type, 0, tempList);

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
					insereTempList(value.varName, value.type, 0, tempList);
					$$.traducao = $1.traducao + $3.traducao + "\t" + $$.label +  " = " +
							$1.label + " % " + $3.label + ";\n";
				}
			}
			| E TK_AND E
			{	
				realizaOperacao($$, $1, $3, " && ");
			}
			
			| E TK_OR E 
			{
				realizaOperacao($$, $1, $3, " || ");
			}
			
			| TK_NOT E
			{
			}
			| TK_INT
			{
				$$.tipo = "int";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, 0, tempList);
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";
			}
			| TK_FLOAT
			{
				$$.tipo = "float";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, 0, tempList);
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";
			}
			| TK_TRUE
			{
				$$.tipo = "int";
				$$.isBool = "bool";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, 0, tempList);
				$$.traducao = "\t" + $$.label + " = " + "1" + ";\n";
			}
			| TK_FALSE
			{
				$$.tipo = "int";
				$$.isBool = "bool";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, 0, tempList);
				$$.traducao = "\t" + $$.label + " = " + "0" + ";\n";
			}
			| TK_CHAR
			{
				$$.tipo = "char";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, 0, tempList);
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";	
					
			}
			| TK_STRING
			{
				cout << "AAAAA\n";
				$$.tipo = "string";
				$$.label =  geraIdAleatorio();
				insereTempList($$.label, $$.tipo, $1.label.size() - 1, tempList);
				$$.traducao = "\t" + string("strcpy(") + $$.label + ", " +  $1.label + ")" + ";\n";		
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
				// insertElement(pilha[indiceEscopoAtual], $1.label, value);
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

