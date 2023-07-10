%{
#include <string>
%}

ESPACO [ \t]
DIGITO [0-9]
LETRA [A-Za-z_]


DELIM {ESPACO}+
NUM {DIGITO}+
REAL {NUM}+("."{NUM}+)?
CHAR \'({LETRA}|{NUM})\'
ID {LETRA}({LETRA}|{NUM})*
STRING \"([^\"\\]|\\.)*\"


%%

"\n"		{}

{DELIM} 	{}

"main"		{return TK_MAIN; }

"if"		{return TK_IF; }

"int"		{return TK_TIPO_INT; }

"float"     {return TK_TIPO_FLOAT; }

"bool"      {return TK_TIPO_BOOL; }

"char"      {return TK_TIPO_CHAR; }

"string"      {return TK_TIPO_STRING; }

"true"      {return TK_TRUE; }

"false"     {return TK_FALSE; }

"(float)"   {return TK_CONVERT_FLOAT; }

"(int)"     {return TK_CONVERT_INT; }

">="        {return TK_GREATER_EQUAL; }

"<="        {return TK_LESS_EQUAL; }

"=="        {return TK_EQUAL_EQUAL; }

"!="        {return TK_NOT_EQUAL; }

"&&"        {return TK_AND; }

"||"        {return TK_OR; }

"!"         {return TK_NOT; }

","         {return TK_VIRGULA; }

"print"     {return TK_PRINT; }

[(){}[\];,]	{ return *yytext; }

[+\-*/=%]   { return *yytext; }

[<>]		{ return *yytext; }


{NUM}		{ yylval.label = yytext; return TK_INT; }
{REAL}      { yylval.label = yytext; return TK_FLOAT; }
{ID}		{ yylval.label = yytext; return TK_ID; }
{CHAR}      { yylval.label = yytext; return TK_CHAR; }
{STRING}      { yylval.label = yytext; return TK_STRING; }
.			{ *yytext; }

%%
