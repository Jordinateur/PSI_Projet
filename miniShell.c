#include <stdio.h>
#include "miniShell.h"
#include "shellUtils.h"

int main(int argc,char **argv){
	char * rawInput;
	char * commandInput;
	char ** lexemes;
	while(1){
		printf("prompt>");
		commandInput = getLine(rawInput);
		lexemes = strToLexeme(commandInput);
		lexemesToCommand(lexemes);
	}
	return 0;
}
