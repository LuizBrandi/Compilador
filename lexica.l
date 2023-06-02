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

%%

"\n"		{}

{DELIM} 	{}

"main"		{return TK_MAIN; }

"int"		{return TK_TIPO_INT; }

"float"     {return TK_TIPO_FLOAT; }

"bool"      {return TK_TIPO_BOOL; }

"true"      {return TK_TRUE; }

">="        {return TK_GREATER_EQUAL; }

"<="        {return TK_LESS_EQUAL; }

"=="        {return TK_EQUAL_EQUAL; }

"!="        {return TK_NOT_EQUAL; }

[(){}[\];,]	{ return *yytext; }

[+\-*/=%]		{ return *yytext; }

[<>]		{ return *yytext; }


{NUM}		{ yylval.label = yytext; return TK_INT; }
{REAL}      { yylval.label = yytext; return TK_FLOAT; }
{ID}		{ yylval.label = yytext; return TK_ID; }
.			{ *yytext; }

%%
