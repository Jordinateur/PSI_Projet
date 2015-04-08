#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "miniShell.h"
#include "shellUtils.h"

char* appendCharToString(char* array, char a)
{
    size_t len = strlen(array);
    char* out;
    out = (char *) malloc(len+2);

    strcpy(out, array);    
    out[len] = a;
    out[len+1] = '\0';

    return out;
}
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

char ** parseLine(char * commandInput){
	char ** out;
	int itemCounter = 0;
	int letterCounter = 0;
	while(*commandInput){
		printf("%c", *commandInput);
		letterCounter++;
		if(*commandInput == ' '){
			printf("\nil s'agit du mot #%d et il y'a #%d lettres\n",itemCounter,letterCounter-1);
			itemCounter++;
			letterCounter = 0;
		}
		commandInput++;
	}
	printf("\nil s'agit du mot #%d et il y'a #%d lettres\n",itemCounter,letterCounter);
	itemCounter++;
}

int main(int argc,char **argv,char **env){
	char * rawInput;
	char * commandInput;
	while(1){
		printf("prompt>");
		commandInput = getLine(rawInput);
		parseLine(commandInput);
	}
	return 0;
}


// int main(int argc,char **argv,char **env){
// 	char commandInput;
// 	char command[255];
// 	int counter = 0;
// 	int loop = 1;
// 	extractPathList(extractPath(env));
// 	while(loop){
// 		printf("prompt$Jordan>");
// 		commandInput = getchar();
// 		if(commandInput != '\n'){
// 			command[counter++] =  commandInput;
// 		}else{
// 			printf("\n\n%s\n\n",command);
// 			counter = 0;
// 		}
// 	}
// 	return 0;
// }
