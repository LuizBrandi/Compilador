%{
#include <iostream>
#include <string>
#include <sstream>

#define YYSTYPE atributos

using namespace std;

// contador pra gerar ids 
int contador = 1;

struct atributos
{
	string label;
	string traducao;
};
//sai
int yylex(void);
void yyerror(string);
string geraIdAleatorio();
%}

%token TK_NUM
%token TK_MAIN TK_ID TK_TIPO_INT
%token TK_FIM TK_ERROR

%start S

%left '+'

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
			|
			;

COMANDO 	: E ';'
			{}
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
			| TK_NUM
			{
				$$.label =  geraIdAleatorio();
				$$.traducao = "\t" + $$.label + " = " + $1.label + ";\n";
			}
			| TK_ID
			{
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
	exit (0);
}				

string geraIdAleatorio(){
	//ai professor, ta muito difícil
	return "var" + to_string(contador++);
}


