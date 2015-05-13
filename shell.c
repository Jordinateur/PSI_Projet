#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "command.h"

//Recupere une chaine de caractere saisie par l'utilisateur
char * readLine(){
	char * rawInput = malloc(sizeof(char *) * 2048);
	memset(rawInput,'\0',sizeof(char *) * 2048);
	char lastChar = getchar();
	int count = 0;
	while(!(lastChar == '\n' || lastChar == EOF) && count < 2048){
		rawInput[count] = lastChar;
		lastChar = getchar();
		count++;

	}
	return rawInput;
}
//Separe la ligne a chaque espace
char ** splitLineToLexemes(char * line){
	char * separator = " \0";
	char ** lexemes = malloc(sizeof(char **) * 255);
	memset(lexemes,0,sizeof(char **) * 255);
	char * lexeme = strtok(line,separator);
	int count = 0;
	while(lexeme != NULL && count < 254){
		lexemes[count] = malloc(sizeof(lexeme)+sizeof(char));
		memset(lexemes[count],'\0',sizeof(lexeme)+sizeof(char));
		strcpy(lexemes[count],lexeme);
		lexeme = strtok(NULL,separator);
		count++;
	}
	count++;
	lexemes[count] = NULL;
	return lexemes;
}
//Remplace les variables d'environement par leurs valeurs
//quentin
char ** replaceVarInLexemes(char ** lexemes){
	char ** out;
	#pragma GCC diagnostic ignored "-Wuninitialized"
	return out;
}

//Separe la chaine de lexemes en chaines de lexemes represantant les instructions pour une commande
char *** splitToCommandLexemesList(char ** lexemes){
	char *** commandLexemesList;
	commandLexemesList = malloc(sizeof(char ***) * 50);
	memset(commandLexemesList,0,sizeof(char ***) * 50);
	int commandCount = 0;
	int lexemeCount = 0;
	while(*lexemes && commandCount < 50){
		if(isCommandDelim(*lexemes)){
			if(!strcmp(*lexemes,"|")){
				commandLexemesList[commandCount][lexemeCount] = malloc(sizeof(*lexemes)+sizeof(char *));
				memset(commandLexemesList[commandCount][lexemeCount],0,sizeof(*lexemes)+sizeof(char *));
				strcpy(commandLexemesList[commandCount][lexemeCount],*lexemes);
				lexemes++;
			}
			commandCount++;
			lexemeCount = 0;
		}
		if(lexemeCount == 0){
			commandLexemesList[commandCount] = malloc(sizeof(char **) * 50);
			memset(commandLexemesList[commandCount],0,sizeof(char **) * 50);
		}
		commandLexemesList[commandCount][lexemeCount] = malloc(sizeof(*lexemes)+sizeof(char *));
		memset(commandLexemesList[commandCount][lexemeCount],0,sizeof(*lexemes)+sizeof(char *));
		strcpy(commandLexemesList[commandCount][lexemeCount],*lexemes);
		lexemeCount++;
		lexemes++;
	}

	return commandLexemesList;
}



//Libere la memoire de la liste de lexemes de commande
void freeCommandLexemesList(char *** commandLexemesList){
	char *** ptr = commandLexemesList;
	while(*ptr){
		void * address = *ptr;
		while(**ptr){
			free(**ptr);
			(*ptr)++;
		}
		free(address);
		ptr++;
	}
	free(commandLexemesList);
}
//Libere la memoire de la liste de lexemes
void freeLexemes(char ** lexemes){
	char ** ptr = lexemes;
	while(*ptr){
		free(*ptr);
		ptr++;
	}
	free(lexemes);
}

int main(int argc, char const *argv[]){
	char * lineEntered = NULL;
	char ** lexemes;
	char *** commandLexemesList;
	command_t ** commandList;
	int loop = 1;
	while(loop){
		printf("Prompt> ");
		lineEntered = readLine();
		lexemes =  splitLineToLexemes(lineEntered);
		free(lineEntered);
		//quentin
		//lexemes = replaceVarInLexemes(lexemes);
		commandLexemesList = splitToCommandLexemesList(lexemes);
		freeLexemes(lexemes);
		commandList = commandLexemesListToCommandList(commandLexemesList);
		executeCommandList(commandList);
		freeCommandLexemesList(commandLexemesList);
		free(commandList);
		//loop--;
	}
	return 0;
}