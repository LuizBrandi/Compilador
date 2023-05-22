all: 	
		clear
		lex lexica.l
		yacc -d sintatica.y
		g++ y.tab.c -o glf -ll

		./glf < exemplo.foca
