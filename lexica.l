%{
#include <string>
%}

ESPACO [ \t]
DIGITO [0-9]
LETRA [A-Za-z_]
DELIM {ESPACO}+
NUM {DIGITO}+
NUM2 {DIGITO}+
REAL {NUM}+("."{NUM}+)?
CHAR \'({LETRA}|{NUM})\'
ID {LETRA}({LETRA}|{NUM})*
STRING \"([^\"\\]|\\.)*\"
PULA_LINHA [\\n]


%%

{DELIM} 	{}

"main"		{return TK_MAIN; }

"if"		{return TK_IF; }

"else"		{return TK_ELSE; }

"switch"    {return TK_SWITCH; }

"case"      {return TK_CASE; }

"default"   {return TK_DEFAULT; }

":"         {return TK_DP; }

"do"        {return TK_DO; }

"while"     {return TK_WHILE; }

"break"     {return TK_BREAK; }

"for"     {return TK_FOR; }

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

"read"      {return TK_READ;}

[(){}[\];,]	{ return *yytext; }

[+\-*/=%]   { return *yytext; }

[<>]		{ return *yytext; }

{NUM2}		 { yylval.label = yytext; return TK_INDEX; }
{NUM}		 { yylval.label = yytext; return TK_INT; }
{REAL}       { yylval.label = yytext; return TK_FLOAT; }
{ID}		 { yylval.label = yytext; return TK_ID; }
{CHAR}       { yylval.label = yytext; return TK_CHAR; }
{STRING}     { yylval.label = yytext; return TK_STRING; }
{PULA_LINHA} { yylval.label = yytext; return TK_PULA_LINHA; }

.			{ *yytext; }

%%
