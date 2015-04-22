#include "shellUtils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

//Recupere sur l'entree standard une chaine de caracter jusqu'au caracter de changement de ligne
char * getLine(char* rawInput){
	char lastChar;
	while((lastChar = getchar()) != '\n'){
		if(rawInput == NULL){
			rawInput = (char *) malloc(1);
			*rawInput = lastChar;
		}else{
			rawInput = appendCharToString(rawInput,lastChar);
		}
	}

	return rawInput;
}
//Lis une chaine de caracter et en ressort un pointeur de pointeur de chaine avec pour chaine les lexemes de cette chaine
char ** strToLexeme(char * str){
	char ** out;
	char * inStr = str;
	char * tmpLexeme = NULL;
	int countLexeme = 0;
	while(*inStr){
		if(*inStr == ' '){
			countLexeme++;
		}
		inStr++;
	}
	if(countLexeme > 0){
		countLexeme++;
		out = (char**) malloc(countLexeme + 1);
	}
	countLexeme = 0;
	inStr = str;
	while(*inStr){
		if(*inStr == ' '){
			out[countLexeme] = (char*) malloc(strlen(tmpLexeme) + 1);
			strcpy(out[countLexeme],tmpLexeme);
			printf("out[%d] = %s\n", countLexeme, tmpLexeme);
			free(tmpLexeme);
			countLexeme++;
		}else{
			tmpLexeme = appendCharToString(tmpLexeme, *inStr);
		}
		inStr++;
	}
	out[countLexeme] = (char*) malloc(strlen(tmpLexeme) + 1);
	strcpy(out[countLexeme],tmpLexeme);
	printf("out[%d] = %s\n", countLexeme, tmpLexeme);
	//out[countLexeme + 1] = malloc(sizeof(char*) * 5);
	out[countLexeme + 1] = NULL;
	printf("a\n");
	if(out[countLexeme + 1] == NULL){
		printf("null\n");
	}
	printf("b\n");
	free(tmpLexeme);

	return out;
}
//Si la commande se termine par un '&'' renvoie vrai
int isBackgrounded(char* binName){
	if(binName[strlen(binName) - 1] == '&'){
		return 1;
	}else{
		return 0;
	}
}
//Detecte si la chaine est une redirection
int isRedirection(char * lexeme){
	char str1[] = "<";
	char str2[] = ">";
	char str3[] = "2>";
	char str4[] = ">&2";
	char str5[] = "2>&1";
	char str6[] = ">>";
	if(strcmp(lexeme,str1) == 0){
		return 1;
	}
	if(strcmp(lexeme,str2) == 0){
		return 1;
	}
	if(strcmp(lexeme,str3) == 0){
		return 1;	
	}
	if(strcmp(lexeme,str4) == 0){
		return 1;
	}
	if(strcmp(lexeme,str5) == 0){
		return 1;
	}
	if(strcmp(lexeme,str5) == 0){
		return 1;
	}
	if(strcmp(lexeme,str6) == 0){
		return 1;
	}
	return 0;
}
//Retourne les ARGV d'un lexeme
char ** extractArgv(char ** lexemes){
	char ** out;
	while(*lexemes != NULL && !isRedirected(*lexemes)){
		out = appendStringToStringList(out,*lexemes);
		lexemes++;
	}
	return out;
}

