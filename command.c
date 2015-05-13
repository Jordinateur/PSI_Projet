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
int redirectToTrash(){
	return fileno(fopen("/dev/null","r+"));
}
pid_t executeCommand(command * c){
	pid_t pid = fork();
	if(pid == 0){
		dup2(c->stdin_ptr,0);
		dup2(c->stdout_ptr,1);
		dup2(c->stderr_ptr,2);
		execv(c->path,c->argv);
		_exit(EXIT_FAILURE);
	}else{
		int status;
		printf("programme  lancee %s ----- pid fils -> %d\n", c->path, pid);
		waitpid(pid, &status, 0);
		if(WIFEXITED(status)){
			c->status = WEXITSTATUS(status);
		}
	}
	return pid;
}
int isCondition(char * lexeme){
	char str1[] = ";";
	char str2[] = "&&";
	char str3[] = "||";
	if(!strcmp(lexeme,str1)) return 1;
	if(!strcmp(lexeme,str2)) return 2;
	if(!strcmp(lexeme,str3)) return 3;
	return 0;
}
//Transforms 
void lexemesToCommand(char ** lexemes, command * c){
	int trash;
	if(isCondition(lexemes[0])){
		switch(isCondition(lexemes[0])){
			case 1:
				c->condition = 0;
				break;
			case 2:
				c->condition = 1;			
				break;
			case 3:
				c->condition = 2;
				break;
		}
		lexemes++;
	}else{
		c->condition = 0;
	}
	if(isBackgrounded(lexemes[0])){
		c->background = 1;
		char * binName = malloc(sizeof(char *) * strlen(lexemes[0]));
		memcpy(binName,lexemes[0],(sizeof(char *) * strlen(lexemes[0]) - 1));
		binName[strlen(lexemes[0]) - 1] = '\0';
		lexemes[0] = binName;
		trash = redirectToTrash();

	}else{
		trash = 0;
		c->background = 0;
	}
	c->path = malloc(sizeof(char *) * 255);
	c->path = findPathForCommand(lexemes[0]);
	c->argv = extractArgv(lexemes);
	c->stdin_ptr = c->background?trash:stdinCommand(lexemes);
	c->stdout_ptr = c->background?trash:stdoutCommand(lexemes);
	c->stderr_ptr = c->background?trash:stderrCommand(lexemes);
	c->status = 0;

	//return c;

}
void freeCommand(command * c){
	free(c->path);
	free(c->argv);
	if(c->background){
		close(c->stdin_ptr);
	}
	//free(c);
}
//Retourne les ARGV d'un lexeme
char ** extractArgv(char ** lexemes){
	int count;
	char ** out;
	char ** ptr = lexemes;
	while(*ptr != NULL && !isRedirection(*ptr)){
		count++;
		ptr++;
	}
	out =  malloc(sizeof(char **) * count + 1);
	memcpy(out,lexemes,sizeof(char **) * count);
	out[count] = "\0";
	return out;
}
//Retourne le chemin d'une commande si elle existe dans le PATH
char * findPathForCommand(char * command){
	char * full_path = getenv("PATH");
	char * path = NULL;
	char * test = NULL;
	char * q_p = command;
	char * quick = malloc(sizeof(char * ) * 2);
	quick[0] = '.';
	quick[1] = '/';
	while(*q_p){
		quick = appendCharToString2(quick,*q_p);
		q_p++;
	}
	if(fileExist(quick)){
		return quick;
	}
	while(*full_path){		
		if(*full_path == ':'){
			test = malloc(sizeof(char) * ((strlen(path) + strlen(command)) + 1));
			strcpy(test,path);
			test = appendCharToString2(test,'/');
			strcat(test,command);
			if(fileExist(test)){
				free(path);
				return test;
			}
			if(test != NULL){
				free(test);
			}
			free(path);
			path = NULL;
		}else{
			path = appendCharToString2(path,*full_path);
		}
		full_path++;
	}
	printf("command not found\n");
	return test;
}