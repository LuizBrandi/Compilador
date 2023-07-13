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


#line 584 "y.tab.c"

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
    TK_IF = 284,
    TK_ELSE = 285,
    TK_DO = 286,
    TK_WHILE = 287
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
#define TK_IF 284
#define TK_ELSE 285
#define TK_DO 286
#define TK_WHILE 287

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
typedef yytype_int8 yy_state_t;

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
#define YYLAST   284

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  9
/* YYNRULES -- Number of rules.  */
#define YYNRULES  50
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  113

#define YYUNDEFTOK  2
#define YYMAXUTOK   287


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
       2,     2,     2,     2,     2,     2,     2,    45,     2,     2,
      39,    40,    37,    35,     2,    36,     2,    38,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    43,
      34,    44,    33,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    41,     2,    42,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   537,   537,   545,   552,   558,   563,   568,   573,   574,
     575,   587,   600,   617,   629,   642,   646,   650,   662,   667,
     675,   768,   808,   860,   920,   977,  1035,  1069,  1075,  1079,
    1083,  1087,  1091,  1103,  1115,  1120,  1124,  1128,  1132,  1136,
    1140,  1160,  1165,  1170,  1173,  1180,  1187,  1195,  1203,  1211,
    1219
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
  "TK_AND", "TK_OR", "TK_NOT", "TK_VIRGULA", "TK_PRINT", "TK_IF",
  "TK_ELSE", "TK_DO", "TK_WHILE", "'>'", "'<'", "'+'", "'-'", "'*'", "'/'",
  "'('", "')'", "'{'", "'}'", "';'", "'='", "'%'", "$accept", "S", "BLOCO",
  "BLOCO_INICIO", "BLOCO_FIM", "COMANDOS", "COMANDO", "COISAS", "E", YY_NULLPTR
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
     285,   286,   287,    62,    60,    43,    45,    42,    47,    40,
      41,   123,   125,    59,    61,    37
};
# endif

#define YYPACT_NINF (-26)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -5,     4,    11,   -24,   -26,   -16,   -25,   -26,   -26,    40,
     -26,   -26,   -26,    15,   -26,   -26,   -26,    -4,    39,    47,
      48,    51,     1,     1,     1,    24,    25,   -25,    26,   -26,
     -26,    40,    64,    27,   258,    35,    36,    49,    50,   -26,
      -3,    -3,   213,     1,     1,    58,     1,    53,   -26,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,   -26,     1,   -26,    60,    62,    63,    65,    73,    90,
     -26,   -26,   -26,   -26,   116,   135,    28,   161,   -26,   155,
     155,   213,   213,   219,   219,    38,    38,   181,   181,    -3,
      -3,   213,   -26,   -26,   -26,   -26,   -26,   -26,     1,    42,
     -25,     1,   -25,   116,    74,    61,   187,   -26,   -26,   -26,
     -25,   -26,   -26
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     0,     4,     2,     7,
      44,    45,    48,     0,    46,    47,    49,    50,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     9,
       5,     7,     0,     0,     0,     0,     0,     0,     0,    50,
      32,    33,    43,     0,     0,     0,     0,     0,     6,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     8,     0,    12,    44,    45,    48,    46,    47,     0,
      10,    11,    13,    14,    27,     0,     0,     0,     3,    35,
      37,    38,    39,    41,    42,    34,    36,    28,    29,    30,
      31,    40,    15,    16,    19,    17,    18,    20,     0,     0,
       0,     0,     0,    27,     0,    22,     0,    24,    26,    21,
       0,    25,    23
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -26,   -26,    -6,   -26,   -26,    87,   -26,    16,   -21
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,    29,     9,    47,    30,    31,    99,    32
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       8,    40,    41,    42,    10,    11,    12,     1,    14,    15,
      16,     4,    39,    69,     3,     5,     7,    22,    23,    51,
      52,    45,    74,    75,     6,    77,    33,    24,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      34,    91,    62,    10,    11,    12,    13,    14,    15,    16,
      35,    17,    18,    19,    20,    21,    22,    23,    36,    37,
      51,    52,    38,    43,    44,    46,    24,   101,    25,    26,
      63,    27,    28,    57,    58,    59,    60,   103,    70,    71,
     106,     7,   104,    62,    49,    50,    51,    52,    53,    54,
      76,   110,    72,    73,   105,    78,   107,    55,    56,    57,
      58,    59,    60,    92,   112,    93,    94,    61,    95,    62,
      49,    50,    51,    52,    53,    54,    96,   109,    48,   108,
       0,     0,     0,    55,    56,    57,    58,    59,    60,     0,
       0,     0,     0,    97,     0,    62,    49,    50,    51,    52,
      53,    54,     0,    98,     0,     0,     0,     0,     0,    55,
      56,    57,    58,    59,    60,    49,    50,    51,    52,    53,
      54,    62,     0,     0,     0,     0,     0,     0,    55,    56,
      57,    58,    59,    60,     0,   100,     0,    51,    52,     0,
      62,    49,    50,    51,    52,    53,    54,     0,    55,    56,
      57,    58,    59,    60,    55,    56,    57,    58,    59,    60,
      62,   102,     0,    51,    52,     0,    62,    49,    50,    51,
      52,    53,    54,     0,     0,     0,     0,     0,    59,    60,
      55,    56,    57,    58,    59,    60,    62,   111,     0,     0,
       0,     0,    62,    49,    50,    51,    52,    53,    54,    49,
      50,    51,    52,     0,     0,     0,    55,    56,    57,    58,
      59,    60,    55,    56,    57,    58,    59,    60,    62,     0,
       0,    64,    65,    66,    62,    67,    68,    16,     0,    39,
       0,     0,     0,     0,    22,    23,     0,     0,     0,     0,
       0,     0,     0,     0,    24
};

static const yytype_int8 yycheck[] =
{
       6,    22,    23,    24,     3,     4,     5,    12,     7,     8,
       9,     0,    11,    34,    10,    39,    41,    16,    17,    22,
      23,    27,    43,    44,    40,    46,    11,    26,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      44,    62,    45,     3,     4,     5,     6,     7,     8,     9,
      11,    11,    12,    13,    14,    15,    16,    17,    11,    11,
      22,    23,    11,    39,    39,    39,    26,    39,    28,    29,
      43,    31,    32,    35,    36,    37,    38,    98,    43,    43,
     101,    41,    40,    45,    20,    21,    22,    23,    24,    25,
      32,    30,    43,    43,   100,    42,   102,    33,    34,    35,
      36,    37,    38,    43,   110,    43,    43,    43,    43,    45,
      20,    21,    22,    23,    24,    25,    43,    43,    31,   103,
      -1,    -1,    -1,    33,    34,    35,    36,    37,    38,    -1,
      -1,    -1,    -1,    43,    -1,    45,    20,    21,    22,    23,
      24,    25,    -1,    27,    -1,    -1,    -1,    -1,    -1,    33,
      34,    35,    36,    37,    38,    20,    21,    22,    23,    24,
      25,    45,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      35,    36,    37,    38,    -1,    40,    -1,    22,    23,    -1,
      45,    20,    21,    22,    23,    24,    25,    -1,    33,    34,
      35,    36,    37,    38,    33,    34,    35,    36,    37,    38,
      45,    40,    -1,    22,    23,    -1,    45,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    37,    38,
      33,    34,    35,    36,    37,    38,    45,    40,    -1,    -1,
      -1,    -1,    45,    20,    21,    22,    23,    24,    25,    20,
      21,    22,    23,    -1,    -1,    -1,    33,    34,    35,    36,
      37,    38,    33,    34,    35,    36,    37,    38,    45,    -1,
      -1,     3,     4,     5,    45,     7,     8,     9,    -1,    11,
      -1,    -1,    -1,    -1,    16,    17,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    12,    47,    10,     0,    39,    40,    41,    48,    49,
       3,     4,     5,     6,     7,     8,     9,    11,    12,    13,
      14,    15,    16,    17,    26,    28,    29,    31,    32,    48,
      51,    52,    54,    11,    44,    11,    11,    11,    11,    11,
      54,    54,    54,    39,    39,    48,    39,    50,    51,    20,
      21,    22,    23,    24,    25,    33,    34,    35,    36,    37,
      38,    43,    45,    43,     3,     4,     5,     7,     8,    54,
      43,    43,    43,    43,    54,    54,    32,    54,    42,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    43,    43,    43,    43,    43,    43,    27,    53,
      40,    39,    40,    54,    40,    48,    54,    48,    53,    43,
      30,    40,    48
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    47,    48,    49,    50,    51,    51,    52,    52,
      52,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    53,    53,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     5,     0,     0,     2,     0,     2,     1,
       3,     3,     3,     3,     3,     4,     4,     4,     4,     4,
       4,     6,     5,     7,     5,     6,     3,     0,     3,     3,
       3,     3,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     2,     1,     1,     1,     1,     1,     1,
       1
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
#line 538 "sintatica.y"
                        {
				cout << "/*Compilador FOCA*/\n" << "#include <iostream>\n#include<string.h>\n#include<stdio.h>\nint main(void)\n{\n";
				printList(tempList); 
				cout << yyvsp[0].traducao << "\treturn 0;\n}" << endl; 
			}
#line 1979 "y.tab.c"
    break;

  case 3:
#line 546 "sintatica.y"
            {
                yyval.traducao = yyvsp[-2].traducao;
            }
#line 1987 "y.tab.c"
    break;

  case 4:
#line 552 "sintatica.y"
            {
                empilha(pilha);
            }
#line 1995 "y.tab.c"
    break;

  case 5:
#line 558 "sintatica.y"
            { 
                desempilha(pilha);
            }
#line 2003 "y.tab.c"
    break;

  case 6:
#line 564 "sintatica.y"
                        {
				yyval.traducao =  yyvsp[-1].traducao + yyvsp[0].traducao;
			}
#line 2011 "y.tab.c"
    break;

  case 7:
#line 568 "sintatica.y"
                        {	
				yyval.traducao = "";
			}
#line 2019 "y.tab.c"
    break;

  case 10:
#line 576 "sintatica.y"
                        {
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], yyvsp[-1].label);
				SYMBOL_TYPE value;
				value.varName = yyvsp[-1].label;
				value.type = "int";
				value.temp = geraIdAleatorio();
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], yyvsp[-1].label, value);
				insereTempList(value.temp, value.type, 0, tempList);
				yyval.traducao = yyvsp[-2].traducao + yyvsp[-1].traducao;
			}
#line 2035 "y.tab.c"
    break;

  case 11:
#line 588 "sintatica.y"
                        {
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], yyvsp[-1].label);
				SYMBOL_TYPE value;
				value.varName = yyvsp[-1].label;
				value.type = "float";
				value.temp = geraIdAleatorio();
				
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], yyvsp[-1].label, value);
				insereTempList(value.temp, value.type, 0, tempList);
				yyval.traducao = yyvsp[-2].traducao + yyvsp[-1].traducao;
			}
#line 2052 "y.tab.c"
    break;

  case 12:
#line 601 "sintatica.y"
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
				insereTempList(value.temp, value.type, 0, tempList);
				
				yyval.traducao = yyvsp[-2].traducao + yyvsp[-1].traducao;
			}
#line 2073 "y.tab.c"
    break;

  case 13:
#line 618 "sintatica.y"
                        {
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], yyvsp[-1].label);
				SYMBOL_TYPE value;
				value.varName = yyvsp[-1].label;
				value.type = "char";
				value.temp = geraIdAleatorio();				
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], yyvsp[-1].label, value);
				insereTempList(value.temp, value.type, 0, tempList);
				yyval.traducao = yyvsp[-2].traducao + yyvsp[-1].traducao;
			}
#line 2089 "y.tab.c"
    break;

  case 14:
#line 630 "sintatica.y"
                        {
				verificaDeclaracaoPrevia(pilha[indiceEscopoAtual], yyvsp[-1].label);
				SYMBOL_TYPE value;
				value.varName = yyvsp[-1].label;
				value.type = "string";
				value.stringSize = "0";
				value.temp = geraIdAleatorio();				
				//insere id na tabela de simbolos
				insertElement(pilha[indiceEscopoAtual], yyvsp[-1].label, value);
				insereTempList(value.temp, value.type, 0, tempList);
				yyval.traducao = yyvsp[-2].traducao + yyvsp[-1].traducao;
			}
#line 2106 "y.tab.c"
    break;

  case 15:
#line 643 "sintatica.y"
                        {
				atribuicao(yyval, yyvsp[-3], yyvsp[-1], pilha);
			}
#line 2114 "y.tab.c"
    break;

  case 16:
#line 647 "sintatica.y"
                        {
				atribuicao(yyval, yyvsp[-3], yyvsp[-1], pilha);
			}
#line 2122 "y.tab.c"
    break;

  case 17:
#line 651 "sintatica.y"
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
#line 2138 "y.tab.c"
    break;

  case 18:
#line 663 "sintatica.y"
                        {
				yyvsp[-1].label = "0";
				atribuicao(yyval, yyvsp[-3], yyvsp[-1], pilha);
			}
#line 2147 "y.tab.c"
    break;

  case 19:
#line 668 "sintatica.y"
                        {
				atribuicao(yyval, yyvsp[-3], yyvsp[-1], pilha);
			}
#line 2155 "y.tab.c"
    break;

  case 20:
#line 676 "sintatica.y"
                        {	
				//o tipo do tk id e do E tem q ser verificado
				bool S1IsId;
				bool S3IsId;
				SYMBOL_TYPE elementS1;
				SYMBOL_TYPE elementS3;
				
				// verifica se é um id ou número na Tabela de Simbolos, true se estiver, false se não estiver
				int indiceS1 = buscaEscopo(pilha, yyvsp[-3].label);
				//true se esta na temp, false se nao esta
				int S1estaNaTemp = procuraNaListaTemp(tempList, yyvsp[-3].label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS1 < 0 && !(S1estaNaTemp)){
					//erro
					yyerror("ERRO!" + yyvsp[-3].label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S1estaNaTemp){
					S1IsId = false;
				}
				
				if(indiceS1 >= 0){
					S1IsId = true;
					elementS1 = returnElement(pilha[indiceS1], yyvsp[-3].label);
				}

				int indiceS3 =  buscaEscopo(pilha, yyvsp[-1].label);
				int S3estaNaTemp = procuraNaListaTemp(tempList, yyvsp[-1].label);

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
					elementS3 = returnElement(pilha[indiceS3], yyvsp[-1].label);
				}
				
				// SYMBOL_TYPE value = returnElement(pilha[indiceS1], $1.label);
				// Se o tipo do TK_ID for igual ao tipo da Expressão, não alteramos o tipo da Expressão, atribuindo normalmente
				
				if(S1IsId == true && S3IsId == true){
					if(elementS1.isBool != "bool" && elementS3.isBool == "bool") yyerror("Atribuição inválida!\n");
					//Caso de ID e ID 
					if((elementS1.type == pilha[indiceS3][yyvsp[-1].label].type)){
						yyval.traducao = yyvsp[-3].traducao + yyvsp[-1].traducao + "\t" + pilha[indiceS1][yyvsp[-3].label].temp + " = " 
						+ pilha[indiceS3][yyvsp[-1].label].temp + ";\n";
					}
					//Caso de ID e ID com tipos diferentes, ou seja, ocorre uma conversão
					if((elementS1.type != pilha[indiceS3][yyvsp[-1].label].type)){
						yyval.traducao = yyvsp[-3].traducao + yyvsp[-1].traducao + "\t" + pilha[indiceS1][yyvsp[-3].label].temp + " = " + 
						"(" + elementS1.type + ")" + pilha[indiceS3][yyvsp[-1].label].temp + ";\n";		
					}

					if(pilha[indiceS1][yyvsp[-3].label].type == "string"){
						cout << "BBBBBB\n";
						yyval.traducao = yyvsp[-3].traducao + yyvsp[-1].traducao + "\t" + 
						pilha[indiceS1][yyvsp[-3].label].temp + " = " + "(char *) malloc(" + pilha[indiceS3][yyvsp[-1].label].stringSize + ")" + ";\n" +
						"\t" + "strcpy(" + pilha[indiceS1][yyvsp[-3].label].temp + ", " + pilha[indiceS3][yyvsp[-1].label].temp + ")"  + ";\n";
					}
				}
				//Caso de ID e TEMP
				if(S1IsId == true && S3IsId == false){
					if(elementS1.isBool != "bool" && yyvsp[-1].isBool == "bool") yyerror("Atribuição inválida!\n");

					//precisamos fazer isso para verificar o tamanho da string
					elementS3 = retornaListaTemp(tempList, yyvsp[-1].label);	

					//Caso de ID e TEMP
					if((elementS1.type == yyvsp[-1].tipo)){
						yyval.traducao = yyvsp[-3].traducao + yyvsp[-1].traducao + "\t" + pilha[indiceS1][yyvsp[-3].label].temp + " = " 
						+ yyvsp[-1].label + ";\n";
					}
					//Caso de ID e TEMP com tipos diferetes, fazendo conversao 
					if((elementS1.type != yyvsp[-1].tipo)){
						yyval.traducao = yyvsp[-3].traducao + yyvsp[-1].traducao + "\t" + pilha[indiceS1][yyvsp[-3].label].temp + " = " + 
						"(" + elementS1.type + ")" + yyvsp[-1].label + ";\n";			
					}

					if(pilha[indiceS1][yyvsp[-3].label].type == "string"){
						pilha[indiceS1][yyvsp[-3].label].stringSize = elementS3.stringSize;
						yyval.traducao = yyvsp[-3].traducao + yyvsp[-1].traducao + "\t" + 
						pilha[indiceS1][yyvsp[-3].label].temp + " = " + "(char *) malloc(" + elementS3.stringSize + ")" + ";\n" +
						"\t" + string("strcpy(") + pilha[indiceS1][yyvsp[-3].label].temp + ", " + yyvsp[-1].label + ")"  + ";\n";
					}
				}		
			}
#line 2252 "y.tab.c"
    break;

  case 21:
#line 769 "sintatica.y"
                        {
				// std::cout << "Exemplo de impressão literal de \\n: \\n\n";
				
				// cout << $3.label + "--------------";
				// if($3.label == "_p") $$.traducao = $3.traducao + "\t" + "cout" + " << " + "endl" + ";\n";

				bool S1IsId;
				bool S3IsId;
				SYMBOL_TYPE elementS1;
				SYMBOL_TYPE elementS3;
				
				// verifica se é um id ou número na Tabela de Simbolos, true se estiver, false se não estiver
				int indiceS1 = buscaEscopo(pilha, yyvsp[-3].label);
				//true se esta na temp, false se nao esta
				int S1estaNaTemp = procuraNaListaTemp(tempList, yyvsp[-3].label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS1 < 0 && !(S1estaNaTemp)){
					//erro
					yyerror("ERRO!" + yyvsp[-3].label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S1estaNaTemp){
					S1IsId = false;
				}
				
				if(indiceS1 >= 0){
					S1IsId = true;
					elementS1 = returnElement(pilha[indiceS1], yyvsp[-3].label);
				}

				//Quando o COISAS TA VAZIO 
				if(yyvsp[-2].label == "") yyval.traducao = yyvsp[-3].traducao + yyvsp[-2].traducao + "\t" + "cout" + " << " + elementS1.temp + ";\n";

				if(yyvsp[-2].label != ""){
					//pegar o que tem no coisas, guardar numa variavel e depois printar
					yyval.traducao = yyvsp[-3].traducao + yyvsp[-2].traducao + "\t" + "cout" + " << " + elementS1.temp + ";\n";	
				} 	
			}
#line 2296 "y.tab.c"
    break;

  case 22:
#line 809 "sintatica.y"
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
					insereTempList(value.temp, value.type, 0, tempList);

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
					insereTempList(value.temp, value.type, 0, tempList);

					yyval.traducao = yyvsp[-2].traducao + "\t" + value.temp + " = !" + yyvsp[-2].label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_IF" + idBloco + ";\n" +
					yyvsp[0].traducao +
					"\t" + "FIM_IF" + idBloco + ":\n";
				}
			}
#line 2352 "y.tab.c"
    break;

  case 23:
#line 861 "sintatica.y"
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
					insereTempList(value.temp, value.type, 0, tempList);

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
					insereTempList(value.temp, value.type, 0, tempList);

					yyval.traducao = yyvsp[-4].traducao + "\t" + value.temp + " = !" + yyvsp[-4].label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "INICIO_ELSE" + idElse + ";\n" +
					yyvsp[-2].traducao +
					"\t" + "goto " + "FIM_IF" + idBloco + ";\n" +
					"\t" + "INICIO_ELSE" + idElse + ":\n" +
					yyvsp[0].traducao +
					"\t" + "FIM_IF" + idBloco + ":\n";
				}
			}
#line 2416 "y.tab.c"
    break;

  case 24:
#line 921 "sintatica.y"
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
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS3.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					yyval.traducao = yyvsp[-2].traducao + "\t" + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + value.temp + " = !" + elementS3.temp + ";\n" +
					"\t" + "if(" + elementS3.temp + ") " + "goto " + "FIM_WHILE" + idWhile + ";\n" +
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
					insereTempList(value.temp, value.type, 0, tempList);

					yyval.traducao = yyvsp[-2].traducao + "\t" + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + value.temp + " = !" + yyvsp[-2].label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_WHILE" + idWhile + ";\n" +
					yyvsp[0].traducao +
					"\t" + "goto " + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + "FIM_WHILE" + idWhile + ":\n"; 
				}
			}
#line 2477 "y.tab.c"
    break;

  case 25:
#line 978 "sintatica.y"
                        {
				bool S5IsId;
				SYMBOL_TYPE elementS5;

				int indiceS5 = buscaEscopo(pilha, yyvsp[-1].label);
				//true se esta na temp, false se nao esta
				bool S5estaNaTemp = procuraNaListaTemp(tempList, yyvsp[-1].label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS5 < 0 && !(S5estaNaTemp)){
					//erro
					yyerror("ERRO!" + yyvsp[-1].label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S5estaNaTemp){
					S5IsId = false;
				}
				
				if(indiceS5 >= 0){
					S5IsId = true;
					elementS5 = returnElement(pilha[indiceS5], yyvsp[-1].label);
				}

				if(S5IsId){

					if(elementS5.isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = elementS5.type;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					yyval.traducao += "\t" + "INICIO_WHILE" + idWhile + ";\n" +
					yyvsp[-4].traducao +
					"\t" + value.temp + " = !" + elementS5.temp + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_WHILE" + idWhile + ";\n" +
					"\t" + "goto " + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + "FIM_WHILE" + idWhile + ":\n"; 
				}

				if(!S5IsId){
					if(yyvsp[-1].isBool != "bool") yyerror("A expressão não é do tipo booleano!\n");
					string idWhile = geraLabelBloco();
					SYMBOL_TYPE value;
					value.type = yyvsp[-3].tipo;
					value.temp = geraIdAleatorio();
					insereTempList(value.temp, value.type, 0, tempList);

					yyval.traducao = yyvsp[-4].traducao + "\t" + "INICIO_WHILE" + idWhile + ";\n" +
					"\t" + value.temp + " = !" + yyvsp[-1].label + ";\n" +
					"\t" + "if(" + value.temp + ") " + "goto " + "FIM_WHILE" + idWhile + ";\n" +
					yyvsp[-4].traducao +
					"\t" + "goto " + "INICIO_WHILE" + idWhile + ";\n";
				}
			}
#line 2537 "y.tab.c"
    break;

  case 26:
#line 1036 "sintatica.y"
                        {	
				bool S1IsId;
				bool S3IsId;
				
				SYMBOL_TYPE element;
				// verifica se é um id ou número na Tabela de Simbolos, true se estiver, false se não estiver
				int indiceS1 = buscaEscopo(pilha, yyvsp[-1].label);
				//true se esta na temp, false se nao esta
				int S1estaNaTemp = procuraNaListaTemp(tempList, yyvsp[-1].label);
				
				// //Se o indice < 0, não está na lista de temps, é uma var não declarada
				if(indiceS1 < 0 && !(S1estaNaTemp)){
					//erro
					yyerror("ERRO!" + yyvsp[0].label + "não foi declarada.");
				}
				// Caso onde o elemento S1 é um 'number', ou seja, um '1' ... '999999'
				if(S1estaNaTemp){
					S1IsId = false;
				}
				
				if(indiceS1 >= 0){
					S1IsId = true;
					element = returnElement(pilha[indiceS1], yyvsp[-1].label);
				}
		
				if(element.type == "int"){
					string valor = element.value;
					int stringSize = valor.size();
					// $$.traducao = $2.traducao + $3.traducao + "\t" + "cout" + " << " + elementS1.temp + ";\n";
				}
				yyval.traducao = yyvsp[-1].traducao + yyvsp[0].traducao;
			}
#line 2574 "y.tab.c"
    break;

  case 27:
#line 1069 "sintatica.y"
                        {
				yyval.traducao = "";
			}
#line 2582 "y.tab.c"
    break;

  case 28:
#line 1076 "sintatica.y"
                        {		
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " + ");				
			}
#line 2590 "y.tab.c"
    break;

  case 29:
#line 1080 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " - ");
			}
#line 2598 "y.tab.c"
    break;

  case 30:
#line 1084 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " * ");
			}
#line 2606 "y.tab.c"
    break;

  case 31:
#line 1088 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " / ");
			}
#line 2614 "y.tab.c"
    break;

  case 32:
#line 1092 "sintatica.y"
                        {
				yyval.tipo = "float";
				yyval.label =  geraIdAleatorio();

				SYMBOL_TYPE tempConvert;
				tempConvert.temp = yyval.label;
				tempConvert.type = yyval.tipo;
				insereTempList(tempConvert.temp, tempConvert.type, 0, tempList);

				yyval.traducao = yyvsp[-1].traducao + yyvsp[0].traducao + "\t" + tempConvert.temp + " = " + "(" + yyval.tipo + ")" + yyvsp[0].label + ";\n";
			}
#line 2630 "y.tab.c"
    break;

  case 33:
#line 1104 "sintatica.y"
                        {
				yyval.tipo = "float";
				yyval.label =  geraIdAleatorio();

				SYMBOL_TYPE tempConvert;
				tempConvert.temp = yyval.label;
				tempConvert.type = yyval.tipo;
				insereTempList(tempConvert.temp, tempConvert.type, 0, tempList);

				yyval.traducao = yyvsp[-1].traducao + yyvsp[0].traducao + "\t" + tempConvert.temp + " = " + "(" + yyval.tipo + ")" + yyvsp[0].label + ";\n";
			}
#line 2646 "y.tab.c"
    break;

  case 34:
#line 1116 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " > ");
				
			}
#line 2655 "y.tab.c"
    break;

  case 35:
#line 1121 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " >= ");
			}
#line 2663 "y.tab.c"
    break;

  case 36:
#line 1125 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " < ");
			}
#line 2671 "y.tab.c"
    break;

  case 37:
#line 1129 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " <= ");	
			}
#line 2679 "y.tab.c"
    break;

  case 38:
#line 1133 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " == ");
			}
#line 2687 "y.tab.c"
    break;

  case 39:
#line 1137 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " != ");	
			}
#line 2695 "y.tab.c"
    break;

  case 40:
#line 1141 "sintatica.y"
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
					insereTempList(value.varName, value.type, 0, tempList);
					yyval.traducao = yyvsp[-2].traducao + yyvsp[0].traducao + "\t" + yyval.label +  " = " +
							yyvsp[-2].label + " % " + yyvsp[0].label + ";\n";
				}
			}
#line 2719 "y.tab.c"
    break;

  case 41:
#line 1161 "sintatica.y"
                        {	
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " && ");
			}
#line 2727 "y.tab.c"
    break;

  case 42:
#line 1166 "sintatica.y"
                        {
				realizaOperacao(yyval, yyvsp[-2], yyvsp[0], " || ");
			}
#line 2735 "y.tab.c"
    break;

  case 43:
#line 1171 "sintatica.y"
                        {
			}
#line 2742 "y.tab.c"
    break;

  case 44:
#line 1174 "sintatica.y"
                        {
				yyval.tipo = "int";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, 0, tempList);
				yyval.traducao = "\t" + yyval.label + " = " + yyvsp[0].label + ";\n";
			}
#line 2753 "y.tab.c"
    break;

  case 45:
#line 1181 "sintatica.y"
                        {
				yyval.tipo = "float";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, 0, tempList);
				yyval.traducao = "\t" + yyval.label + " = " + yyvsp[0].label + ";\n";
			}
#line 2764 "y.tab.c"
    break;

  case 46:
#line 1188 "sintatica.y"
                        {
				yyval.tipo = "int";
				yyval.isBool = "bool";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, 0, tempList);
				yyval.traducao = "\t" + yyval.label + " = " + "1" + ";\n";
			}
#line 2776 "y.tab.c"
    break;

  case 47:
#line 1196 "sintatica.y"
                        {
				yyval.tipo = "int";
				yyval.isBool = "bool";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, 0, tempList);
				yyval.traducao = "\t" + yyval.label + " = " + "0" + ";\n";
			}
#line 2788 "y.tab.c"
    break;

  case 48:
#line 1204 "sintatica.y"
                        {
				yyval.tipo = "char";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, 0, tempList);
				yyval.traducao = "\t" + yyval.label + " = " + yyvsp[0].label + ";\n";	
					
			}
#line 2800 "y.tab.c"
    break;

  case 49:
#line 1212 "sintatica.y"
                        {
				cout << "AAAAA\n";
				yyval.tipo = "string";
				yyval.label =  geraIdAleatorio();
				insereTempList(yyval.label, yyval.tipo, yyvsp[0].label.size() - 1, tempList);
				yyval.traducao = "\t" + string("strcpy(") + yyval.label + ", " +  yyvsp[0].label + ")" + ";\n";		
			}
#line 2812 "y.tab.c"
    break;

  case 50:
#line 1220 "sintatica.y"
                        {
				yyval.traducao =  "";
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
#line 2832 "y.tab.c"
    break;


#line 2836 "y.tab.c"

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
#line 1236 "sintatica.y"


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

