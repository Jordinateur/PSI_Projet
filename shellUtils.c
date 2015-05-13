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
		rawInput = appendCharToString2(rawInput,lastChar);
	}
	return rawInput;
}
//Lis une chaine de caracter et en ressort un pointeur de pointeur de chaine avec pour chaine les lexemes de cette chaine
char ** strToLexemes(char * str){
	char ** out;
	char * arg;
	char count = 0;
	char * str_ptr = str;
	while(*str_ptr){
		if(*str_ptr == ' ') count++;
		str_ptr++;
	}
	out = malloc(sizeof(char **) * (count + 2));
	arg = strtok(str, " ");
	int i = 0;
    while( arg != NULL ){
    	out[i] = malloc(sizeof(char * ) * (strlen(arg) + 1));
		strcpy(out[i],arg);
    	arg = strtok(NULL, " ");
    	i++;
	}
	out[i] = NULL;
	return out;
}
int isEndCommand(char * lexeme){
	char str1[] = ";";
	char str2[] = "||";
	char str3[] = "&&";


	if(!strcmp(lexeme,str1) || !strcmp(lexeme,str2) || !strcmp(lexeme,str3)){
		return 1;
	}
	return 0;
}

int * splitPointLexemes(char ** lexemes){
	int * out;
	int count = 0;
	int commandCount = 0;
	char ** tmp = lexemes;
	while(tmp && *tmp){
		if(isEndCommand(*tmp)){
			commandCount++;
		}
		tmp++;
	}
	out = malloc(sizeof(int *) * (commandCount + 2));
	commandCount = 0;
	while(lexemes && *lexemes){
		if(isEndCommand(*lexemes)){
			out[commandCount] = count;
			commandCount++;
		}		
		count++;
		lexemes++;
	}
	out[commandCount] = count;
	out[commandCount+1] = '\0';
	return out;
}

char *** lexemesToCommands(char ** lexemes,int * splitPoints){
	int start = 0;
	int end = splitPoints[0];
	int commandCount = 0;
	int * sp = splitPoints;
	char *** out;
	while(*sp){
		commandCount++;
		
		sp++;
	}
	out = malloc(sizeof(command *) * (commandCount + 2));
	commandCount = 0;
	while(*splitPoints){
		end = *splitPoints;
		out[commandCount] = malloc(sizeof(char **) * (end - start));
		while(start < end){
			out[commandCount] = appendStringToStringList2(out[commandCount],lexemes[start]);
			start++;
		}
		commandCount++;
		start = end;
		splitPoints++;
	}
	out[commandCount] = 0;
	return out;

}