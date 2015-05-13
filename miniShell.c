#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "command.h"
#include <stdlib.h>
#include "miniShell.h"
#include "shellUtils.h"
int main(int argc,char **argv){
	char * rawInput = malloc(sizeof(char * ));
	int *  test;
	rawInput[0] = '\0';
	char * commandInput;
	char ** lexemes;
	char *** co;
	command * c;
	printf("Debut %d\n", getpid());
	int i = 1;
	while(i){
		printf("prompt>");
		//On lit une chaine de caractere termine par \n ou EOF
		commandInput = getLine(rawInput);
		//On transforme cette chaine en char ** la separation se faisant sur les espaces
		lexemes = strToLexemes(commandInput);
		//Recupere un tableau marquant les positions des separateur de commandes
		test = splitPointLexemes(lexemes);
		//Cree un char *** contenant toutes les lexemes de chaques commandes
		co = lexemesToCommands(lexemes,test);
		int lastStatus = 1;
		int stop = 0;
		//On cree les command dans un command * ;
		while(*co && !stop){
			c = malloc(sizeof(command *));
			lexemesToCommand(*co, c);
			if(!c->condition){
				executeCommand(c);
			}else{
				if(c->condition == 2){
					if(lastStatus){
						executeCommand(c);
					}else{
						stop = 1;
					}
				}else{
					if(!lastStatus){
						executeCommand(c);
					}else{
						stop = 1;
					}
				}
			}
			lastStatus = c->status;
			freeCommand(c);
			co++;
		}
		char ** l = lexemes;
		while(*l){
			free(*l);
			l++;
		}
		free(lexemes);
		i--;
	}
	return 0;
}
