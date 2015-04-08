#include "shellUtils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* extractPath(char **env){
	char pathWord[5] = "PATH";
	while(env++){
		if(strstr(*env,pathWord) != NULL)
			return *env;
	}
}
void extractPathList(char* path){
	char *rawPath = &(path[4]);
	char out[50][255];
	char currentPath[255];
	int nbElement = 0;
	int charCounter = 0;
	while(*(rawPath++)){
		int newElement = 0;
		if(*rawPath == ':'){
			newElement = 1;
			charCounter = 0;
		}else{
 			out[nbElement][charCounter] = *rawPath;
			charCounter++;
		}
		if(newElement){
			nbElement++;
			newElement = 0;
		}
	}
	int c = 0;
	while(c++ <= nbElement){
		//Iteration sur le tableau contenant tous les chemins du PATH
		//printf("%s\n\n",out[c]);
	}
}