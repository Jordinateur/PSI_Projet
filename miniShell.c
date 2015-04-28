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
	rawInput[0] = '\0';
	char * commandInput;
	char ** lexemes;
	pid_t rid = getpid();
	printf("Test -----\n");
	printf("Debut %d\n", getpid());
	while(1 && rid != 0){
		printf("prompt>");
		commandInput = getLine(rawInput);
		lexemes = strToLexeme(commandInput);
		rid = executeCommand(lexemesToCommand(lexemes));
	}
	printf("Test -----\n");
	return 0;
}
