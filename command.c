#include "command.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
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
int isIn(char * lexeme){
	char str1[] = "<";
	return !strcmp(lexeme,str1);
}
int isErr(char * lexeme){
	char str3[] = "2>";
	int r = 0;
	r = r || strcmp(lexeme,str3);
	return r;
}
int isOut(char * lexeme){
	char str2[] = ">";
	char str3[] = "2>";
	char str4[] = ">&2";
	char str5[] = "2>&1";
	char str6[] = ">>";
	int r = 0;
	r = r || strcmp(lexeme,str2);
	r = r || strcmp(lexeme,str3);
	r = r || strcmp(lexeme,str4);
	r = r || strcmp(lexeme,str5);
	r = r || strcmp(lexeme,str5);
	r = r || strcmp(lexeme,str6);
	return r;
}
//Find a stdin redirection
int stdinCommand(char ** lexemes){
	while(*lexemes){
		if(isRedirection(*lexemes)){
			if(isIn(*lexemes)){
				lexemes++;
				if(fileExist(*lexemes)){
					return fileno(fopen(*lexemes, "r"));	
				}
			}
		}
		lexemes++;
	}
	return STDIN_FILENO;
}
//Find a stdout redirection
int stdoutCommand(char ** lexemes){
	char str2[] = ">";
	//char str4[] = ">&2";
	//char str5[] = "2>&1";
	char str6[] = ">>";
	while(*lexemes){
		if(isRedirection(*lexemes)){
			if(isOut(*lexemes)){
				printf("Compare ->  %s\n", *lexemes);
				if(!strcmp(str2,*lexemes)){
					lexemes++;
					return fileno(fopen(*lexemes, "w"));
				}else{
					if(!strcmp(str6,*lexemes)){
						lexemes++;
						return fileno(fopen(*lexemes, "a"));	
					}
				}
			}
		}
		lexemes++;
	}
	return STDOUT_FILENO;
}
//Find a stderr redirection
int stderrCommand(char ** lexemes){
	char str3[] = "2>";

	while(*lexemes){
		if(isRedirection(*lexemes)){
			if(isErr(*lexemes)){
				if(strcmp(str3,*lexemes)){
					lexemes++;
					return fileno(fopen(*lexemes, "a"));
				}
			}
		}
		lexemes++;
	}
	return STDERR_FILENO;
}
pid_t executeCommand(command * c){
	pid_t pid = fork();
	if(pid == 0){
		dup2(c->stdin_ptr,0);
		dup2(c->stdout_ptr,1);
		dup2(c->stderr_ptr,2);
		printf("Fils %d\n \n", c->status);
		c->status  = c->status + 1;
		execv(c->path,c->argv);
		_exit(EXIT_FAILURE);
	}else{
		int status;
		waitpid(pid, &status, 0);
		printf("Fin du Fils Pere %d \n \n", c->status);
	}
	return pid;
}
//Transforms 
command * lexemesToCommand(char ** lexemes){
	command * c = malloc(sizeof(command));
	if(isBackgrounded(lexemes[0])){
		lexemes[0][strlen(lexemes[0]) - 1] = '\0';
		c->background = 1;
	}else{
		c->background = 0;
	}
	c->path = findPathForCommand(lexemes[0]);
	c->argv = extractArgv(lexemes);
	c->stdin_ptr = stdinCommand(lexemes);
	c->stdout_ptr = stdoutCommand(lexemes);
	c->stderr_ptr = stderrCommand(lexemes);
	c->status = 0;

	return c;

}
//Retourne les ARGV d'un lexeme
char ** extractArgv(char ** lexemes){
	char ** out;
	while(*lexemes != NULL && !isRedirection(*lexemes)){
		out = appendStringToStringList(out,*lexemes);
		lexemes++;
	}
	return out;
}
//Retourne le chemin d'une commande si elle existe dans le PATH
char * findPathForCommand(char * command){
	char * full_path = getenv("PATH");
	char * path = NULL;
	char * test = NULL;
	char * q_p = command;
	char * quick = malloc(sizeof(char * ));
	quick[0] = '\0';
	quick = appendCharToString(quick,'.');
	quick = appendCharToString(quick,'/');
	while(*q_p){
		quick = appendCharToString(quick,*q_p);
		q_p++;
	}
	printf("quick -> %s\n", quick);
	if(fileExist(quick)){
		printf("command located -> %s\n", quick);
		return quick;
	}
	while(*full_path){		
		if(*full_path == ':'){
			test = malloc(sizeof(char) * ((strlen(path) + strlen(command)) + 1));
			strcpy(test,path);
			test = appendCharToString(test,'/');
			strcat(test,command);
			if(fileExist(test)){
				free(path);
				printf("command located -> %s\n", test);
				return test;
			}
			if(test != NULL){
				free(test);
			}
			free(path);
			path = NULL;
		}else{
			path = appendCharToString(path,*full_path);
		}
		full_path++;
	}
	printf("command not found\n");
	return test;
}