%{
#include <stdio.h>

void yyerror(char *); /* ver abaixo */
%}

//Definindo tokens
%token INTEIRO
%token FIM_LINHA

//Definindo o início da leitura
%start linha


/*Regras de Derivação*/
%%

/*Linha leva numa expressão*/
/*Seguindo o esquema de árvore ascendente,
a expressão vai subir levando dados pra regra que tá
implicando ela (linha), depois que ela 'ler' o FIM_LINHA
ele toma a ação { printf("valor: %d\n", $1); }
lembre que nessa regra, $$ = linha e $1 = expresssao,
e expressao guardava o resultado final da operação */
//$$ =    $1
linha: expressao FIM_LINHA { printf("valor: %d\n", $1); }
     ;

/*expressao leva numa expressao '+' termo que quando
ela é formada por completo ela gera a ação: { $$ = $1 + $3; } 
OU expressao leva num termo, e um termo gera ação: { $$ = $1; }*/

/*Quando ele termina de 'ler' a folha que seria o termo
ele verifica se tenho uma regra que de expressao que precisa
de mais coisas da entrada, e ve que sim, o que faz com que ele
peça mais dados da entrada*/
//$$          $1     $2   $3
expressao: expressao '+' termo { $$ = $1 + $3; }
        /*na árvore o termo recebeu o valor do inteiro
        na regra abaixo, depois disso, o Parser já analisa
        pra ver se 'termo' é implicância em alguma outra coisa
        assim ele reconhece que expressao implica em termo*/
         | termo { $$ = $1; }
         ;

/*$$ é o 'pai' ou a regra que levou em outra,
por exemplo: o $$ */  

/*Termo leva num INTEIRO, que gera ação { $$ = $1; }*/

/*Como a árvore é ascendente, ele vai começar 'lendo'
as folhas da árvore, então quando ele achar um INT (ex: 1),
que seria o termo levando em INTEIRO, ele toma uma ação*/
/* Esse INTEIRO foi definido aqui, mas ele tem relação com
o lexica.l, pois lá ele reconhece um digito, e retorna o 
token INTEIRO com o valor do digito digitado na entrada*/
//$$    $1
termo: INTEIRO { $$ = $1; }
     ;

%%

int main(int argc, char **argv)
{
  return yyparse();
}

/* função usada pelo bison para dar mensagens de erro */
void yyerror(char *msg)
{
  fprintf(stderr, "erro: %s\n", msg);
}

