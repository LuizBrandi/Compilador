/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "sintatica.y"

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
	string valorAtributo;
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
	bool isVect = false;
	string vectSize;
} SYMBOL_TYPE;

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
		if(list[i].type != "string"){
			if(list[i].isVect) cout << "\t" << list[i].type << " " << list[i].varName << "[" << list[i].vectSize << "]" << ";\n";
			if(!list[i].isVect) cout << "\t" << list[i].type << " " << list[i].varName << ";\n";
		}
	}
} 
		

void insereTempList(string label, string tipo, int tamanhoString, bool isVect, string vectSize,  vector<SYMBOL_TYPE>& tempList){
	SYMBOL_TYPE temp;
	temp.varName = label;
	temp.type = tipo;
	temp.stringSize = to_string(tamanhoString);
	temp.isVect = isVect;
	temp.vectSize = vectSize;
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
		yyerror("ERRO!" + $1.label + " não foi declarada.");
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
		insereTempList(tempConvert.temp, tempConvert.type, 0, false, "0",  tempList);
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
		insereTempList(tempConvert.temp, tempConvert.type, 0, false, "0",  tempList);
		

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
	insereTempList(value.temp, value.type, 0, false, "0",  tempList);
	
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
						vector<unordered_map<string, SYMBOL_TYPE>>& pilha, vector<SYMBOL_TYPE>& tempList){
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


#line 709 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TK_INT = 258,
    TK_FLOAT = 259,
    TK_CHAR = 260,
    TK_TIPO_BOOL = 261,
    TK_TRUE = 262,
    TK_FALSE = 263,
    TK_STRING = 264,
    TK_MAIN = 265,
    TK_ID = 266,
    TK_TIPO_INT = 267,
    TK_TIPO_FLOAT = 268,
    TK_TIPO_CHAR = 269,
    TK_TIPO_STRING = 270,
    TK_CONVERT_FLOAT = 271,
    TK_CONVERT_INT = 272,
    TK_FIM = 273,
    TK_ERROR = 274,
    TK_GREATER_EQUAL = 275,
    TK_LESS_EQUAL = 276,
    TK_EQUAL_EQUAL = 277,
    TK_NOT_EQUAL = 278,
    TK_AND = 279,
    TK_OR = 280,
    TK_NOT = 281,
    TK_VIRGULA = 282,
    TK_PRINT = 283,
    TK_READ = 284,
    TK_IF = 285,
    TK_ELSE = 286,
    TK_DO = 287,
    TK_WHILE = 288,
    TK_FOR = 289,
    TK_BREAK = 290,
    TK_SWITCH = 291,
    TK_CASE = 292,
    TK_DEFAULT = 293,
    TK_DP = 294
  };
#endif
/* Tokens.  */
#define TK_INT 258
#define TK_FLOAT 259
#define TK_CHAR 260
#define TK_TIPO_BOOL 261
#define TK_TRUE 262
#define TK_FALSE 263
#define TK_STRING 264
#define TK_MAIN 265
#define TK_ID 266
#define TK_TIPO_INT 267
#define TK_TIPO_FLOAT 268
#define TK_TIPO_CHAR 269
#define TK_TIPO_STRING 270
#define TK_CONVERT_FLOAT 271
#define TK_CONVERT_INT 272
#define TK_FIM 273
#define TK_ERROR 274
#define TK_GREATER_EQUAL 275
#define TK_LESS_EQUAL 276
#define TK_EQUAL_EQUAL 277
#define TK_NOT_EQUAL 278
#define TK_AND 279
#define TK_OR 280
#define TK_NOT 281
#define TK_VIRGULA 282
#define TK_PRINT 283
#define TK_READ 284
#define TK_IF 285
#define TK_ELSE 286
#define TK_DO 287
#define TK_WHILE 288
#define TK_FOR 289
#define TK_BREAK 290
#define TK_SWITCH 291
#define TK_CASE 292
#define TK_DEFAULT 293
#define TK_DP 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   497

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  156

#define YYUNDEFTOK  2
#define YYMAXUTOK   294


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    54,     2,     2,
      46,    47,    44,    42,     2,    43,     2,    45,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    50,
      41,    53,    40,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    51,     2,    52,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    48,     2,    49,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   663,   663,   671,   677,   682,   689,   695,   718,   725,
     730,   737,   743,   813,   831,   832,   833,   846,   859,   876,
     888,   901,   946,   950,   954,   966,   971,   979,   983,   998,
    1019,  1072,  1132,  1174,  1229,  1271,  1283,  1289,  1306,  1312,
    1327,  1331,  1335,  1339,  1352,  1364,  1369,  1373,  1377,  1381,
    1385,  1389,  1410,  1415,  1420,  1423,  1432,  1439,  1447,  1455,
    1463,  1471
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_INT", "TK_FLOAT", "TK_CHAR",
  "TK_TIPO_BOOL", "TK_TRUE", "TK_FALSE", "TK_STRING", "TK_MAIN", "TK_ID",
  "TK_TIPO_INT", "TK_TIPO_FLOAT", "TK_TIPO_CHAR", "TK_TIPO_STRING",
  "TK_CONVERT_FLOAT", "TK_CONVERT_INT", "TK_FIM", "TK_ERROR",
  "TK_GREATER_EQUAL", "TK_LESS_EQUAL", "TK_EQUAL_EQUAL", "TK_NOT_EQUAL",
  "TK_AND", "TK_OR", "TK_NOT", "TK_VIRGULA", "TK_PRINT", "TK_READ",
  "TK_IF", "TK_ELSE", "TK_DO", "TK_WHILE", "TK_FOR", "TK_BREAK",
  "TK_SWITCH", "TK_CASE", "TK_DEFAULT", "TK_DP", "'>'", "'<'", "'+'",
  "'-'", "'*'", "'/'", "'('", "')'", "'{'", "'}'", "';'", "'['", "']'",
  "'='", "'%'", "$accept", "S", "BLOCO", "COMANDOS", "BLOCO_INICIO",
  "BLOCO_FIM", "BLOCO_SWITCH", "CASE_LISTA", "CASE_STMT", "CASE",
  "COMANDO", "COISAS", "E", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
      62,    60,    43,    45,    42,    47,    40,    41,   123,   125,
      59,    91,    93,    61,    37
};
# endif

#define YYPACT_NINF (-47)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -3,     0,    11,   -34,   -47,   -33,   -35,   -47,   -47,    69,
     -47,   -47,   -47,     4,   -47,   -47,   -47,   -37,     9,    10,
      13,    14,   455,   455,   455,   -23,   -19,   -17,   -35,   -15,
     -14,    -2,     5,   -47,   -47,    69,   100,    15,   471,   -46,
      17,    18,    19,   -47,    -4,    -4,   407,   455,    48,   455,
      27,   455,    50,   -47,   455,    21,   -47,   455,   455,   455,
     455,   455,   455,   455,   455,   455,   455,   455,   455,   -47,
     455,   -47,    29,    37,    38,    39,    41,   135,   -47,   455,
     -47,   -47,   -47,   170,    16,   178,    25,   206,    40,   214,
     -47,    12,    12,   407,   407,   401,   401,   129,   129,    84,
      84,    -4,    -4,   407,   -47,   -47,   -47,   -47,   -47,   -47,
     242,   455,    45,    59,   -35,   455,   -35,   455,    52,    61,
     170,    62,   -47,    31,   268,   -47,   296,   -30,   -47,   -47,
     -47,   -47,   -35,    66,   455,   455,    79,    78,   -30,   -47,
     -47,   -47,   331,   366,    69,   -47,   -47,    53,    69,   -47,
      77,   -47,   455,   372,   -35,   -47
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     6,     2,     5,
      55,    56,    59,     0,    57,    58,    60,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    15,     7,     5,     0,     0,     0,     0,
       0,     0,     0,    61,    43,    44,    54,     0,     0,     0,
       0,     0,     0,    36,     0,     0,     4,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    14,
       0,    18,    55,    56,    59,    57,    58,     0,    16,     0,
      17,    19,    20,    38,     0,     0,     0,     0,     0,     0,
       3,    46,    48,    49,    50,    52,    53,    45,    47,    39,
      40,    41,    42,    51,    22,    23,    26,    24,    25,    27,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      38,     0,    29,    30,     0,    32,     0,    10,    35,    21,
      37,    28,     0,     0,     0,     0,     0,     0,    10,    11,
      31,    33,     0,     0,     5,     8,     9,     0,     5,    13,
       0,    12,     0,     0,     0,    34
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -47,   -47,    -6,   -29,   -47,   -47,   -47,    -5,   -47,   -47,
     -47,    26,   -21
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    33,    34,     9,    55,   128,   137,   138,   139,
      35,   112,    36
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
       8,    44,    45,    46,    78,    79,    56,   135,   136,     1,
       3,     4,     5,     7,     6,    37,    38,    77,    59,    60,
      39,    40,    50,    47,    41,    42,    83,    48,    85,    49,
      87,    51,    52,    89,    59,    60,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,    53,   103,
      70,    54,    63,    64,    65,    66,    67,    68,   110,    84,
      86,    88,   132,   113,   150,    71,    70,    80,    81,    82,
      90,   115,    10,    11,    12,    13,    14,    15,    16,   104,
      17,    18,    19,    20,    21,    22,    23,   105,   106,   107,
     120,   108,   121,   117,   124,    24,   126,    25,    26,    27,
     127,    28,    29,    30,    31,    32,    59,    60,   123,   122,
     125,   129,   131,   142,   143,   149,   141,     7,   144,   151,
      57,    58,    59,    60,    61,    62,   140,   145,    67,    68,
     152,   153,     0,   146,     0,     0,     0,     0,    70,     0,
      63,    64,    65,    66,    67,    68,   130,     0,   155,     0,
      69,    59,    60,     0,    70,    57,    58,    59,    60,    61,
      62,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,    66,    67,    68,    63,    64,    65,    66,    67,
      68,     0,     0,    70,     0,   109,     0,     0,     0,    70,
      57,    58,    59,    60,    61,    62,     0,   111,    57,    58,
      59,    60,    61,    62,     0,     0,     0,     0,     0,     0,
      63,    64,    65,    66,    67,    68,     0,     0,    63,    64,
      65,    66,    67,    68,    70,   114,    57,    58,    59,    60,
      61,    62,    70,     0,    57,    58,    59,    60,    61,    62,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,    68,     0,   116,    63,    64,    65,    66,    67,    68,
      70,   118,    57,    58,    59,    60,    61,    62,    70,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    63,    64,    65,    66,    67,    68,    57,    58,
      59,    60,    61,    62,   119,     0,    70,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,    64,
      65,    66,    67,    68,     0,   133,    57,    58,    59,    60,
      61,    62,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    63,    64,    65,    66,
      67,    68,     0,     0,     0,     0,   134,     0,     0,     0,
      70,    57,    58,    59,    60,    61,    62,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,    68,     0,     0,     0,
       0,   147,     0,     0,     0,    70,    57,    58,    59,    60,
      61,    62,    57,    58,    59,    60,    61,    62,     0,     0,
       0,     0,     0,     0,     0,   148,    63,    64,    65,    66,
      67,    68,    63,    64,    65,    66,    67,    68,     0,   154,
      70,    57,    58,    59,    60,     0,    70,    57,    58,    59,
      60,    61,    62,     0,     0,     0,     0,     0,     0,     0,
       0,    63,    64,    65,    66,    67,    68,    63,    64,    65,
      66,    67,    68,     0,     0,    70,     0,     0,    10,    11,
      12,    70,    14,    15,    16,     0,    43,     0,     0,     0,
       0,    22,    23,     0,    72,    73,    74,     0,    75,    76,
      16,    24,    43,     0,     0,     0,     0,    22,    23,     0,
       0,     0,     0,     0,     0,     0,     0,    24
};

static const yytype_int16 yycheck[] =
{
       6,    22,    23,    24,    50,    51,    35,    37,    38,    12,
      10,     0,    46,    48,    47,    11,    53,    38,    22,    23,
      11,    11,    28,    46,    11,    11,    47,    46,    49,    46,
      51,    46,    46,    54,    22,    23,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    50,    70,
      54,    46,    40,    41,    42,    43,    44,    45,    79,    11,
      33,    11,    31,    47,    11,    50,    54,    50,    50,    50,
      49,    46,     3,     4,     5,     6,     7,     8,     9,    50,
      11,    12,    13,    14,    15,    16,    17,    50,    50,    50,
     111,    50,    47,    53,   115,    26,   117,    28,    29,    30,
      48,    32,    33,    34,    35,    36,    22,    23,   114,    50,
     116,    50,    50,   134,   135,   144,    50,    48,    39,   148,
      20,    21,    22,    23,    24,    25,   132,    49,    44,    45,
      53,   152,    -1,   138,    -1,    -1,    -1,    -1,    54,    -1,
      40,    41,    42,    43,    44,    45,   120,    -1,   154,    -1,
      50,    22,    23,    -1,    54,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    43,    44,    45,    40,    41,    42,    43,    44,
      45,    -1,    -1,    54,    -1,    50,    -1,    -1,    -1,    54,
      20,    21,    22,    23,    24,    25,    -1,    27,    20,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      40,    41,    42,    43,    44,    45,    -1,    -1,    40,    41,
      42,    43,    44,    45,    54,    47,    20,    21,    22,    23,
      24,    25,    54,    -1,    20,    21,    22,    23,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,
      44,    45,    -1,    47,    40,    41,    42,    43,    44,    45,
      54,    47,    20,    21,    22,    23,    24,    25,    54,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    41,    42,    43,    44,    45,    20,    21,
      22,    23,    24,    25,    52,    -1,    54,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,
      42,    43,    44,    45,    -1,    47,    20,    21,    22,    23,
      24,    25,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    42,    43,
      44,    45,    -1,    -1,    -1,    -1,    50,    -1,    -1,    -1,
      54,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    41,    42,    43,    44,    45,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    54,    20,    21,    22,    23,
      24,    25,    20,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    40,    41,    42,    43,
      44,    45,    40,    41,    42,    43,    44,    45,    -1,    47,
      54,    20,    21,    22,    23,    -1,    54,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    40,    41,    42,    43,    44,    45,    40,    41,    42,
      43,    44,    45,    -1,    -1,    54,    -1,    -1,     3,     4,
       5,    54,     7,     8,     9,    -1,    11,    -1,    -1,    -1,
      -1,    16,    17,    -1,     3,     4,     5,    -1,     7,     8,
       9,    26,    11,    -1,    -1,    -1,    -1,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    12,    56,    10,     0,    46,    47,    48,    57,    59,
       3,     4,     5,     6,     7,     8,     9,    11,    12,    13,
      14,    15,    16,    17,    26,    28,    29,    30,    32,    33,
      34,    35,    36,    57,    58,    65,    67,    11,    53,    11,
      11,    11,    11,    11,    67,    67,    67,    46,    46,    46,
      57,    46,    46,    50,    46,    60,    58,    20,    21,    22,
      23,    24,    25,    40,    41,    42,    43,    44,    45,    50,
      54,    50,     3,     4,     5,     7,     8,    67,    50,    51,
      50,    50,    50,    67,    11,    67,    33,    67,    11,    67,
      49,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    50,    50,    50,    50,    50,    50,
      67,    27,    66,    47,    47,    46,    47,    53,    47,    52,
      67,    47,    50,    57,    67,    57,    67,    48,    61,    50,
      66,    50,    31,    47,    50,    37,    38,    62,    63,    64,
      57,    50,    67,    67,    39,    49,    62,    50,    39,    58,
      11,    58,    53,    67,    47,    57
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    55,    56,    57,    58,    58,    59,    60,    61,    62,
      62,    63,    64,    64,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    66,    66,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67,    67,    67,    67,    67,    67,    67,    67,    67,
      67,    67
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     5,     2,     0,     0,     0,     3,     2,
       0,     1,     4,     3,     2,     1,     3,     3,     3,     3,
       3,     6,     4,     4,     4,     4,     4,     4,     6,     5,
       5,     7,     5,     7,    13,     5,     2,     3,     0,     3,
       3,     3,     3,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     1,     1,     1,     1,     1,
       1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 664 "sintatica.y"
                        {
				cout << "/*Compilador FOCA*/\n" << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\nint main(void)\n{\n";
				printList(tempList); 
				cout << yyvsp[0].traducao << "\treturn 0;\n}" << endl; 
			}
#line 2182 "y.tab.c"
    break;

  case 3:
#line 672 "sintatica.y"
            {
                yyval.traducao = yyvsp[-2].traducao;
            }
#line 2190 "y.tab.c"
    break;

  case 4:
#line 678 "sintatica.y"
                        {
				yyval.traducao =  yyvsp[-1].traducao + yyvsp[0].traducao;
			}
#line 2198 "y.tab.c"
    break;

  case 5:
#line 682 "sintatica.y"
                        {	
				yyval.traducao = "";
			}
#line 2206 "y.tab.c"
    break;

  case 6:
#line 689 "sintatica.y"
            {
                empilha(pilha);
            }
#line 2214 "y.tab.c"
    break;

  case 7:
#line 695 "sintatica.y"
            { 
                desempilha(pilha);
            }
#line 2222 "y.tab.c"
    break;

  case 8:
#line 719 "sintatica.y"
            {
                // Faça o processamento necessário para o bloco switch
                yyval.traducao += yyvsp[-1].traducao;
            }
#line 2231 "y.tab.c"
    break;

  case 9:
#line 726 "sintatica.y"
            {
                yyval.traducao += yyvsp[0].traducao;
            }
#line 2239 "y.tab.c"
    break;

  case 10:
#line 730 "sintatica.y"
            {
                // Caso base: não há mais casos na lista
                yyval.traducao = "";
            }
#line 2248 "y.tab.c"
    break;

  case 11:
#line 738 "sintatica.y"
            {
                // Faça o processamento necessário para o caso
            }
#line 2256 "y.tab.c"
    break;

  case 12:
#line 744 "sintatica.y"
            {
				bool S2isId;
				SYMBOL_TYPE elementS2;
				int indiceS2;
				bool S2estaNaTemp;
				
				verificaExpressao(yyvsp[-2], S2isId, S2estaNaTemp, elementS2, indiceS2, pilha, tempList);
				
				if(S2isId){
					string idSwitch = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS2.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);
					

					yyval.traducao = yyvsp[-2].traducao + "\t" + "INICIO_CASE" + idSwitch + ":\n" +
					"\t" + value.temp + " = !" + elementS2.temp + ";\n" +
					yyvsp[0].traducao +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_SWITCH" + ";\n";
				}
				if(!S2isId){
					string idSwitch = geraLabelBloco();	
					SYMBOL_TYPE value;
					value.type = yyvsp[-2].tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);
					pilhaLoop.push(value.temp);

					yyval.traducao = yyvsp[-2].traducao + "\t" + "INICIO_CASE" + idSwitch + ":\n" +
					"\t" + value.temp + " = !" + yyvsp[-2].label + ";\n" +
					yyvsp[0].traducao +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_SWITCH" + ";\n";
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
#line 2330 "y.tab.c"
    break;

  case 13:
#line 814 "sintatica.y"
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
#line 2348 "y.tab.c"
    break;

  case 16:
#line 834 "sintatica.y"
                        {
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], yyvsp[-1].label);
				SYMBOL_TYPE value;
				value.varName = yyvsp[-1].label;
				value.type = "int";
				value.temp = geraIdAleatorio();
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], yyvsp[-1].label, value);
				
				insereTempList(value.temp, value.type, 0, false, "0",  tempList);
				yyval.traducao = yyvsp[-2].traducao + yyvsp[-1].traducao;
			}
#line 2365 "y.tab.c"
    break;

  case 17:
#line 847 "sintatica.y"
                        {
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], yyvsp[-1].label);
				SYMBOL_TYPE value;
				value.varName = yyvsp[-1].label;
				value.type = "float";
				value.temp = geraIdAleatorio();
				
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], yyvsp[-1].label, value);
				insereTempList(value.temp, value.type, 0, false, "0",  tempList);
				yyval.traducao = yyvsp[-2].traducao + yyvsp[-1].traducao;
			}
#line 2382 "y.tab.c"
    break;

  case 18:
#line 860 "sintatica.y"
                        {
				// $$.teste = 'Z';
				// $2.label = "pastel de frango 2 reais";
				// $2.isBool = 1;
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], yyvsp[-1].label);
				SYMBOL_TYPE value;
				value.varName = yyvsp[-1].label;
				value.type = "int";
				value.isBool = "bool";
				value.temp = geraIdAleatorio();
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], yyvsp[-1].label, value);
				insereTempList(value.temp, value.type, 0, false, "0",  tempList);
				
				yyval.traducao = yyvsp[-2].traducao + yyvsp[-1].traducao;
			}
#line 2403 "y.tab.c"
    break;

  case 19:
#line 877 "sintatica.y"
                        {
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], yyvsp[-1].label);
				SYMBOL_TYPE value;
				value.varName = yyvsp[-1].label;
				value.type = "char";
				value.temp = geraIdAleatorio();				
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], yyvsp[-1].label, value);
				insereTempList(value.temp, value.type, 0, false, "0",  tempList);
				yyval.traducao = yyvsp[-2].traducao + yyvsp[-1].traducao;
			}
#line 2419 "y.tab.c"
    break;

  case 20:
#line 889 "sintatica.y"
                        {
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], yyvsp[-1].label);
				SYMBOL_TYPE value;
				value.varName = yyvsp[-1].label;
				value.type = "string";
				value.stringSize = "0";
				value.temp = geraIdAleatorio();				
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], yyvsp[-1].label, value);
				insereTempList(value.temp, value.type, 0, false, "0",  tempList);
				yyval.traducao = yyvsp[-2].traducao + yyvsp[-1].traducao;
			}
#line 2436 "y.tab.c"
    break;

  case 21:
#line 902 "sintatica.y"
                        {
				//Verica declaracao da variavel
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], yyvsp[-4].label);
				
				bool S4IsId;
				SYMBOL_TYPE elementS4;
				int indiceS4;
				bool S4isInTemp;
				
				//Verificando a expressao
				verificaExpressao(yyvsp[-2], S4IsId, S4isInTemp, elementS4, indiceS4, pilha, tempList);

				//quando é ID
				if(S4IsId) if(elementS4.type != "int") yyerror("ERRO: índice inválido!");
				
				if(!S4IsId) if(yyvsp[-2].tipo != "int") yyerror("ERRO: índice inválido!");
					
				
				SYMBOL_TYPE value;
				value.varName = yyvsp[-4].label;
				value.type = "int";
				value.temp = geraIdAleatorio();
				value.isVect = true;

				if(S4IsId){
					cout << "TESTESSSSSSSS\n";
					value.vectSize = elementS4.temp;
				}
				cout << "TESTESSSSSSSS " << yyvsp[-2].label << "\n"; 
				if(!S4IsId) value.vectSize = yyvsp[-2].valorAtributo;

				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], yyvsp[-4].label, value);
				//VETOR
				insereTempList(value.temp, value.type, 0, value.isVect, value.vectSize,  tempList);
				

				//Se é ID pega pela tabela de simbolos
				if(S4IsId) yyval.traducao = yyvsp[-5].traducao + yyvsp[-4].traducao;
				//Se é ID pega pelo S4LABEL
				if(!S4IsId) yyval.traducao = yyvsp[-5].traducao + yyvsp[-4].traducao;
				
	
			}
#line 2485 "y.tab.c"
    break;

  case 22:
#line 947 "sintatica.y"
                        {
				atribuicao(yyval, yyvsp[-3], yyvsp[-1], pilha);
			}
#line 2493 "y.tab.c"
    break;

  case 23:
#line 951 "sintatica.y"
                        {
				atribuicao(yyval, yyvsp[-3], yyvsp[-1], pilha);
			}
#line 2501 "y.tab.c"
    break;

  case 24:
#line 955 "sintatica.y"
                        {
				string valor = "1";
				//procurando o elemento nos varios escopo
				int aux = buscaEscopo(pilha, yyvsp[-3].label);
				
				if(aux < 0 ){
					yyerror("ERROR:" + yyvsp[-3].label + " não foi declarado.");
				}
				pilha[aux][yyvsp[-3].label].isBool = "bool";
				yyval.traducao = yyvsp[-3].traducao + yyvsp[-1].traducao + "\t" + pilha[aux][yyvsp[-3].label].temp + " = " + valor + ";\n";
			}
#line 2517 "y.tab.c"
    break;

  case 25:
#line 967 "sintatica.y"
                        {
				yyvsp[-1].label = "0";
				atribuicao(yyval, yyvsp[-3], yyvsp[-1], pilha);
			}
#line 2526 "y.tab.c"
    break;

  case 26:
#line 972 "sintatica.y"
                        {
				atribuicao(yyval, yyvsp[-3], yyvsp[-1], pilha);
			}
#line 2534 "y.tab.c"
    break;

  case 27:
#line 980 "sintatica.y"
                        {	
				tkIDAtribuicao(yyval, yyvsp[-3], yyvsp[-1], pilha);
			}
#line 2542 "y.tab.c"
    break;

  case 28:
#line 984 "sintatica.y"
                        {
				bool S3IsId;
				bool S3isInTemp;
				int indiceS3;
				SYMBOL_TYPE elementS3;

				verificaExpressao(yyvsp[-3], S3IsId, S3isInTemp, elementS3, indiceS3, pilha, tempList);
				
				//quando não é ID
				if(!S3IsId) yyval.traducao += yyvsp[-3].traducao + yyvsp[-2].traducao + "\t"  + "cout" + " << " + yyvsp[-3].label + ";\n";

				//quando é ID
				if(S3IsId) yyval.traducao += yyvsp[-3].traducao + yyvsp[-2].traducao + "\t" + "cout" + " << " + elementS3.temp + ";\n";
			}
#line 2561 "y.tab.c"
    break;

  case 29:
#line 999 "sintatica.y"
                        {
				bool S3IsId;
				bool S3isInTemp;
				int indiceS3;
				SYMBOL_TYPE elementS3;

				

				verificaExpressao(yyvsp[-2], S3IsId, S3isInTemp, elementS3, indiceS3, pilha, tempList);
				
				//quando não é ID
				if(!S3IsId) yyerror("ERRO: Leitura Inválida, o Admin será notificado.\n");

				//quando é ID
				
				if(S3IsId){
					if(elementS3.type == "string") yyval.traducao += "\t" + elementS3.temp + " = " + "(char *) malloc(" + "10000" + ")" + ";\n";
					yyval.traducao += yyvsp[-2].traducao + yyvsp[-1].traducao + "\t" + "cin" + " >> " + elementS3.temp + ";\n";
				}
			}
#line 2586 "y.tab.c"
    break;

  case 30:
#line 1020 "sintatica.y"
                        {
				bool S3IsId;
				SYMBOL_TYPE elementS3;

				int indiceS3 = buscaEscopo(pilha, yyvsp[-2].label);
				//true se esta na temp, false se nao esta
				bool S3estaNaTemp = procuraNaListaTemp(tempList, yyvsp[-2].label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS3 < 0 && !(S3estaNaTemp)){
					//erro
					yyerror("ERRO!" + yyvsp[-2].label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S3estaNaTemp){
					S3IsId = false;
				}
				
				if(indiceS3 >= 0){
					S3IsId = true;
					elementS3 = returnElement(pilha[indiceS3], yyvsp[-2].label);
				}

				if(S3IsId){
					if(elementS3.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idBloco = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS3.type;
					value.temp = geraIdAleatorio();
					
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);

					yyval.traducao = yyvsp[-2].traducao + "\t" + value.temp + " = !" + elementS3.temp + ";\n" +
					"\t" + "if(" + elementS3.temp + ") " + "goto " + "FIM_IF" + idBloco + ";\n" +
					yyvsp[0].traducao +
					"\t" + "FIM_IF" + idBloco + ":\n";
				}

				if(!S3IsId){
					if(yyvsp[-2].isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idBloco = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = yyvsp[-2].tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);

					yyval.traducao = yyvsp[-2].traducao + "\t" + value.temp + " = !" + yyvsp[-2].label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_IF" + idBloco + ";\n" +
					yyvsp[0].traducao +
					"\t" + "FIM_IF" + idBloco + ":\n";
				}
			}
#line 2643 "y.tab.c"
    break;

  case 31:
#line 1073 "sintatica.y"
                        {
				bool S3IsId;
				SYMBOL_TYPE elementS3;

				int indiceS3 = buscaEscopo(pilha, yyvsp[-4].label);
				//true se esta na temp, false se nao esta
				bool S3estaNaTemp = procuraNaListaTemp(tempList, yyvsp[-4].label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS3 < 0 && !(S3estaNaTemp)){
					//erro
					yyerror("ERRO!" + yyvsp[-4].label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S3estaNaTemp){
					S3IsId = false;
				}
				
				if(indiceS3 >= 0){
					S3IsId = true;
					elementS3 = returnElement(pilha[indiceS3], yyvsp[-4].label);
				}

				if(S3IsId){
					if(elementS3.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idBloco = geraLabelBloco();
					string idElse = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS3.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);

					yyval.traducao = yyvsp[-4].traducao + "\t" + value.temp + " = !" + elementS3.temp + ";\n" +
					"\t" + "if(" + elementS3.temp + ") " + "goto " + "INICIO_ELSE" + idElse + ";\n" +
					yyvsp[-2].traducao +
					"\t" + "goto " + "FIM_IF" + idBloco + ";\n" +
					"\t" + "INICIO_ELSE" + idElse + ":\n" +
					yyvsp[0].traducao +
					"\t" + "FIM_IF" + idBloco + ":\n";
				}

				if(!S3IsId){
					if(yyvsp[-4].isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idBloco = geraLabelBloco();
					string idElse = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = yyvsp[-4].tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);

					yyval.traducao = yyvsp[-4].traducao + "\t" + value.temp + " = !" + yyvsp[-4].label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "INICIO_ELSE" + idElse + ";\n" +
					yyvsp[-2].traducao +
					"\t" + "goto " + "FIM_IF" + idBloco + ";\n" +
					"\t" + "INICIO_ELSE" + idElse + ":\n" +
					yyvsp[0].traducao +
					"\t" + "FIM_IF" + idBloco + ":\n";
				}
			}
#line 2707 "y.tab.c"
    break;

  case 32:
#line 1133 "sintatica.y"
                        {
				bool S3IsId;
				SYMBOL_TYPE elementS3;
				int indiceS3;
				bool S3estaNaTemp;
				// void verificaExpressao(atributos& $, bool& isID, bool& isInTemp, SYMBOL_TYPE& element, int& indicePilha,
				// 		vector<unordered_map<string, SYMBOL_TYPE>>& pilha, unordered_map<string, SYMBOL_TYPE>& hash)

				verificaExpressao(yyvsp[-2], S3IsId, S3estaNaTemp, elementS3, indiceS3, pilha, tempList);

				if(S3IsId){
					if(elementS3.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS3.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);

					yyval.traducao = yyvsp[-2].traducao + "\t" + "INICIO_WHILE" + idWhile + ":\n" +
					"\t" + value.temp + " = !" + elementS3.temp + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_WHILE" + idWhile + ";\n" +
					yyvsp[0].traducao +
					"\t" + "goto " + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + "FIM_WHILE" + idWhile + ":\n";
				}
				if(!S3IsId){
					if(yyvsp[-2].isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = yyvsp[-2].tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);

					yyval.traducao = yyvsp[-2].traducao + "\t" + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + value.temp + " = !" + yyvsp[-2].label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_WHILE" + idWhile + ";\n" +
					yyvsp[0].traducao +
					"\t" + "goto " + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + "FIM_WHILE" + idWhile + ":\n"; 
				}
			}
#line 2753 "y.tab.c"
    break;

  case 33:
#line 1175 "sintatica.y"
                        {
				bool S5IsId;
				SYMBOL_TYPE elementS5;

				int indiceS5 = buscaEscopo(pilha, yyvsp[-2].label);
				//true se esta na temp, false se nao esta
				bool S5estaNaTemp = procuraNaListaTemp(tempList, yyvsp[-2].label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS5 < 0 && !(S5estaNaTemp)){
					//erro
					yyerror("ERRO!" + yyvsp[-2].label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S5estaNaTemp){
					S5IsId = false;
				}
				
				if(indiceS5 >= 0){
					S5IsId = true;
					elementS5 = returnElement(pilha[indiceS5], yyvsp[-2].label);
				}

				if(S5IsId){
					if(elementS5.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS5.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);

					yyval.traducao = "\tINICIO_DOWHILE" + idWhile + ";\n" +
					yyvsp[-5].traducao +
					"\t" + value.temp + " = !" + elementS5.temp + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_DOWHILE" + idWhile + ";\n" +
					"\t" + "goto " + "INICIO_DOWHILE" + idWhile + ";\n" +
					"\t" + "FIM_DOWHILE" + idWhile + ":\n"; 
				}

				if(!S5IsId){
					if(yyvsp[-2].isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = yyvsp[-4].tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);

					yyval.traducao = yyvsp[-5].traducao + "\t" + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + value.temp + " = !" + yyvsp[-2].label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_WHILE" + idWhile + ";\n" +
					yyvsp[-5].traducao +
					"\t" + "goto " + "INICIO_WHILE" + idWhile + ";\n";
				}
			}
#line 2812 "y.tab.c"
    break;

  case 34:
#line 1230 "sintatica.y"
                        {
				bool S7isId;
				SYMBOL_TYPE elementS7;

				int indiceS7 = buscaEscopo(pilha, yyvsp[-6].label);
				//true se esta na temp, false se nao esta
				bool S7EstaNaTemp = procuraNaListaTemp(tempList, yyvsp[-6].label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS7 < 0 && !(S7EstaNaTemp)){
					//erro
					yyerror("ERRO!" + yyvsp[-6].label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S7EstaNaTemp){
					S7isId = false;
				}
				
				if(indiceS7 >= 0){
					S7isId = true;
					elementS7 = returnElement(pilha[indiceS7], yyvsp[-6].label);
				}
				cout << "FUNCAO\n";
				
				if(!S7isId){
					if(yyvsp[-6].isBool != "bool") yyerror("---A expressão não é do tipo booleano!\n");
					string idFor = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = yyvsp[-6].tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, false, "0",  tempList);
					tkIDAtribuicao(yyval, yyvsp[-10], yyvsp[-8], pilha);
					yyval.traducao +=  yyvsp[-10].traducao + yyvsp[-6].traducao + "\t" + "INICIO_FOR" + idFor + ":\n" +
					"\t" + value.temp + " = !" + yyvsp[-6].label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_FOR" + idFor + ";\n" +
					yyvsp[0].traducao;
					tkIDAtribuicao(yyval, yyvsp[-10], yyvsp[-2], pilha); 
					yyval.traducao += "\tgoto INICIO_FOR" + idFor + ";\n" +
					"\t" + "FIM_FOR" + idFor + ":\n"; 
				}
			}
#line 2858 "y.tab.c"
    break;

  case 35:
#line 1272 "sintatica.y"
                        {
				bool S3isId;
				SYMBOL_TYPE elementS3;
				int indiceS3;
				bool S3estaNaTemp;
				
				verificaExpressao(yyvsp[-2], S3isId, S3estaNaTemp, elementS3, indiceS3, pilha, tempList);
				string valueTemp = pilhaLoop.top();

				yyval.traducao = yyvsp[-2].traducao + yyvsp[0].traducao +  "\t" + "FIM_SWITCH:\n";
			}
#line 2874 "y.tab.c"
    break;

  case 36:
#line 1284 "sintatica.y"
                        {
				yyval.traducao = "";
			}
#line 2882 "y.tab.c"
    break;

  case 37:
#line 1290 "sintatica.y"
                        {	
				bool S2IsId;
				bool S2isInTemp;
				int indiceS2;
				SYMBOL_TYPE elementS2;

				verificaExpressao(yyvsp[-1], S2IsId, S2isInTemp, elementS2, indiceS2, pilha, tempList);
				
				//quando não é ID
				if(!S2IsId) yyval.traducao = yyvsp[0].traducao + yyvsp[-1].traducao +  "\t" + "cout" + " << " + yyvsp[-1].label + ";\n";

				//quando é ID
				if(S2IsId) yyval.traducao = yyvsp[-1].traducao +  "\t" + "cout" + " << " + elementS2.temp + ";\n";
				
			}
#line 2902 "y.tab.c"
    break;

  case 38:
#line 1306 "sintatica.y"
                        {
				yyval.traducao = "";
			}
#line 2910 "y.tab.c"
    break;

  case 39:
#line 1313 "sintatica.y"
                        {		
				if (yyvsp[-2].tipo != "string" && yyvsp[-1].tipo != "string") { 
 					realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " + ");				
				} else {
					cout << "tipo $1:" + yyvsp[-2].tipo + "\n";
					cout << "tipo $3:" + yyvsp[0].tipo + "\n";
					//soma de strings (CONCATENACAO)	
					if(yyvsp[-2].tipo == "string" || yyvsp[0].tipo == "string") {
						yyval.traducao = yyvsp[-2].traducao + yyvsp[0].traducao + "\t" + "strcat(" + yyvsp[-2].label  + ", " + yyvsp[0].label + ")" + ";\n";	
					}
				}
				

			}
#line 2929 "y.tab.c"
    break;

  case 40:
#line 1328 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " - ");
			}
#line 2937 "y.tab.c"
    break;

  case 41:
#line 1332 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " * ");
			}
#line 2945 "y.tab.c"
    break;

  case 42:
#line 1336 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " / ");
			}
#line 2953 "y.tab.c"
    break;

  case 43:
#line 1340 "sintatica.y"
                        {
				yyval.tipo = "float";
				yyval.label =  geraIdAleatorio();

				SYMBOL_TYPE tempConvert;
				tempConvert.temp = yyval.label;
				tempConvert.type = yyval.tipo;
				insereTempList(tempConvert.temp, tempConvert.type, 0, false, "0",  tempList);
				

				yyval.traducao = yyvsp[-1].traducao + yyvsp[0].traducao + "\t" + tempConvert.temp + " = " + "(" + yyval.tipo + ")" + yyvsp[0].label + ";\n";
			}
#line 2970 "y.tab.c"
    break;

  case 44:
#line 1353 "sintatica.y"
                        {
				yyval.tipo = "float";
				yyval.label =  geraIdAleatorio();

				SYMBOL_TYPE tempConvert;
				tempConvert.temp = yyval.label;
				tempConvert.type = yyval.tipo;
				insereTempList(tempConvert.temp, tempConvert.type, 0, false, "0",  tempList);

				yyval.traducao = yyvsp[-1].traducao + yyvsp[0].traducao + "\t" + tempConvert.temp + " = " + "(" + yyval.tipo + ")" + yyvsp[0].label + ";\n";
			}
#line 2986 "y.tab.c"
    break;

  case 45:
#line 1365 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " > ");
				
			}
#line 2995 "y.tab.c"
    break;

  case 46:
#line 1370 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " >= ");
			}
#line 3003 "y.tab.c"
    break;

  case 47:
#line 1374 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " < ");
			}
#line 3011 "y.tab.c"
    break;

  case 48:
#line 1378 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " <= ");	
			}
#line 3019 "y.tab.c"
    break;

  case 49:
#line 1382 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " == ");
			}
#line 3027 "y.tab.c"
    break;

  case 50:
#line 1386 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " != ");	
			}
#line 3035 "y.tab.c"
    break;

  case 51:
#line 1390 "sintatica.y"
                        {
				//criando temporaria que recebera a soma
				SYMBOL_TYPE value;

				int caso = 0;
				
				if(yyvsp[-2].tipo != "int"){
					yyerror("Não é possível realizar a operação  " + yyvsp[-2].label + " deve ser do tipo \"int\"\n");
				}else if(yyvsp[0].tipo != "int"){
					yyerror("Não é possível realizar a operação  " + yyvsp[0].label + " deve ser do tipo \"int\"\n");
				} else{
					yyval.label = geraIdAleatorio();	
					value.varName = yyval.label;
					value.type =  "int";
					
					insereTempList(value.varName, value.type, 0, false, "0", tempList);
					yyval.traducao = yyvsp[-2].traducao + yyvsp[0].traducao + "\t" + yyval.label +  " = " +
							yyvsp[-2].label + " % " + yyvsp[0].label + ";\n";
				}
			}
#line 3060 "y.tab.c"
    break;

  case 52:
#line 1411 "sintatica.y"
                        {	
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " && ");
			}
#line 3068 "y.tab.c"
    break;

  case 53:
#line 1416 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " || ");
			}
#line 3076 "y.tab.c"
    break;

  case 54:
#line 1421 "sintatica.y"
                        {
			}
#line 3083 "y.tab.c"
    break;

  case 55:
#line 1424 "sintatica.y"
                        {
				yyval.tipo = "int";
				yyval.label =  geraIdAleatorio();
				yyval.valorAtributo = yyvsp[0].label;
				insereTempList(yyval.label, yyval.tipo, 0, false, "0",  tempList);
				
				yyval.traducao = "\t" + yyval.label + " = " + yyvsp[0].label + ";\n";
			}
#line 3096 "y.tab.c"
    break;

  case 56:
#line 1433 "sintatica.y"
                        {
				yyval.tipo = "float";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, 0, false, "0",  tempList);
				yyval.traducao = "\t" + yyval.label + " = " + yyvsp[0].label + ";\n";
			}
#line 3107 "y.tab.c"
    break;

  case 57:
#line 1440 "sintatica.y"
                        {
				yyval.tipo = "int";
				yyval.isBool = "bool";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, 0, false, "0",  tempList);
				yyval.traducao = "\t" + yyval.label + " = " + "1" + ";\n";
			}
#line 3119 "y.tab.c"
    break;

  case 58:
#line 1448 "sintatica.y"
                        {
				yyval.tipo = "int";
				yyval.isBool = "bool";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, 0, false, "0",  tempList);
				yyval.traducao = "\t" + yyval.label + " = " + "0" + ";\n";
			}
#line 3131 "y.tab.c"
    break;

  case 59:
#line 1456 "sintatica.y"
                        {
				yyval.tipo = "char";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, 0, false, "0",  tempList);
				yyval.traducao = "\t" + yyval.label + " = " + yyvsp[0].label + ";\n";	
					
			}
#line 3143 "y.tab.c"
    break;

  case 60:
#line 1464 "sintatica.y"
                        {
				cout << "AAAAA\n";
				yyval.tipo = "string";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, yyvsp[0].label.size() - 1, false, "0", tempList);
				yyval.traducao = "\t" + string("strcpy(") + yyval.label + ", " +  yyvsp[0].label + ")" + ";\n";		
			}
#line 3155 "y.tab.c"
    break;

  case 61:
#line 1472 "sintatica.y"
                        {
				yyval.traducao =  "";
			}
#line 3163 "y.tab.c"
    break;


#line 3167 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1476 "sintatica.y"


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

