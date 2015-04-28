#include "command.h"
#include "shellUtils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

//Recupere sur l'entree standard une chaine de caracter jusqu'au caracter de changement de ligne
char * getLine(char* rawInput){
	char lastChar;
	while(!((lastChar = getchar()) == '\n' || lastChar == EOF)){
		rawInput = appendCharToString(rawInput,lastChar);
	}
	return rawInput;
}
//Lis une chaine de caracter et en ressort un pointeur de pointeur de chaine avec pour chaine les lexemes de cette chaine
char ** strToLexeme(char * str){
	printf("strToLexeme enter\n");
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
//Transform une chaine de lexemes en les separant en lexemes de commands
char ** lexemesToLexemesCommand(char ** lexemes){
	char ** out;


	return out;

}

