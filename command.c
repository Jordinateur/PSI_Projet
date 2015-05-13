#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

static int runningProcess = 0;
static int pausedProcess = 0;


//Fonction qui envoie le signal d'interruption au processus courant
void sigHandlerInt(int sig){
	kill(runningProcess,SIGINT);
}
//Fonction qui pause/depause un processus
void sigHandlerStp(int sig){
	if(!pausedProcess){
		printf("Process -> %d <- Paused\n", runningProcess);
		kill(runningProcess,SIGTSTP);
		pausedProcess = runningProcess;
		runningProcess = 0;
	}else{
		printf("Process -> %d <- Resumed\n", pausedProcess);
		kill(pausedProcess,SIGCONT);
		runningProcess = pausedProcess;
		pausedProcess = 0;
	}
}
//Verifie si un fichier existe renvoie 1 si oui
int fileExist(char * fpath){

    if(*fpath && access( fpath, F_OK ) != -1 ) {
        return 1;
    } else {
        return 0;
    }
}
//fonction de comparaison avec un delimiteur de fin de commande renvoie 1 si vrai
int isCommandDelim(char * lexeme){
	char str1[] = ";";
	char str2[] = "&&";
	char str3[] = "||";
	char str4[] = "|";
	int r = 0;
	r = r || !strcmp(lexeme,str1);
	r = r || !strcmp(lexeme,str2);
	r = r || !strcmp(lexeme,str3);
	r = r || !strcmp(lexeme,str4);
	return r;
}

//fonction de comparaison avec un delimiteur de fin de commande determine par un numero le delimiteur
int whichCondition(char * lexeme){
	char str1[] = ";";
	char str2[] = "&&";
	char str3[] = "||";
	char str4[] = "|";
	if(!strcmp(lexeme,str1)) return 1;
	if(!strcmp(lexeme,str2)) return 2;
	if(!strcmp(lexeme,str3)) return 3;
	if(!strcmp(lexeme,str4)) return 4;
	return 0;
}
//Si lexeme est egale a "<" renvoi 1
int isInRedirection(char * lexeme){
	char str1[] = "<";
	return !strcmp(lexeme,str1);
}
//Si lexeme correspond a un symbole de redirection de stderr renvoie 1
int isErrRedirection(char * lexeme){
	char str1[] = "2>";
	char str2[] = "2>>";
	int r = 0;
	r = r || !strcmp(lexeme,str1);
	r = r || !strcmp(lexeme,str2);
	return r;
}
//Si lexeme correspond a un symbole de redirection de stdout renvoie 1
int isOutRedirection(char * lexeme){
	char str1[] = ">";
	char str2[] = "1>";
	char str3[] = "1>>";
	char str4[] = ">>";
	int r = 0;
	r = r || !strcmp(lexeme,str1);
	r = r || !strcmp(lexeme,str2);
	r = r || !strcmp(lexeme,str3);
	r = r || !strcmp(lexeme,str4);
	return r;
}
//A partir des fonctions de comparaisons precedentes determine si lexeme est une redirection renvoie 1 si c'est le cas
int isRedirection(char * lexeme){
	int r = 0;
	r = r || isInRedirection(lexeme);
	r = r || isErrRedirection(lexeme);
	r = r || isOutRedirection(lexeme);
	return r;
}
//A partir d'un char ** extrait toute la partie precedent une redirection ou une delimintation de commande et la renvoie
char ** extractArgv(char ** lexemes){
	int count = 0;
	char ** ptr = lexemes;
	char ** argv;
	while(*ptr && !isRedirection(*ptr) && !isCommandDelim(*ptr)){
		count++;
		ptr++;
	}
	argv = malloc(sizeof(char **) * (count + 1));
	memset(argv,0,sizeof(char **) * (count + 1));
	memcpy(argv,lexemes,sizeof(char **) * count);
	return argv;
}
//A partir d'un char * cherche dans la variable PATH si un fichier existe dans tous les chemins, sinon cherche localement renvoie la valeur trouve ou NULL
char * findPath(char * lexeme){
	char * PATH = malloc(sizeof(char *) * 2048);
	strcpy(PATH,getenv("PATH"));
	char * split = ":\0";
	char * current = strtok(PATH,split);
	char * test = malloc(sizeof(char*) * 255);
	memset(test,'\0',sizeof(test));
	while(current != NULL && !fileExist(test)){
		strcpy(test,current);
		strcat(test,"/");
		strcat(test,lexeme);
		current = strtok(NULL,split);
	}
	free(PATH);
	if(current == NULL){
		char currentDir[] = "./";
		strcpy(test,currentDir);
		strcat(test,lexeme);
		if(fileExist(test)){
			return test;
		}else{
			free(test);
			return current;
		}
	}else{
		return test;
	}
}
//Renvoie un descripteur de fichier sur /dev/null (fichier poubelle)
int redirectToTrash(){
	return fileno(fopen("/dev/null","r+"));
}
//Indique a partir d'un lexeme de commande si la commande est pipe a une autre (la suivant)
void handlePipe(char ** lexemes,command_t * c){
	int in = 0;
	c->piped_in = 0;
	c->piped_out = 0;
	while(*lexemes){
		if(!strcmp(*lexemes,"|")){
			c->piped_out = 1;
			free(*lexemes);
			*lexemes = NULL;
			return;
		}
		lexemes++;
		in++;
	}
}
//Gere la presence du caracter & en fin de commande et en deuxieme position dans le lexeme afin d'indiquer si la commande doit etre executer en arriere plan
void handleBackgrounded(char ** lexemes, command_t * c){
	c->background = 0;
	if(lexemes[0] && lexemes[0][strlen(lexemes[0])-1] == '&'){
		lexemes[0][strlen(lexemes[0])-1] = '\0';
		c->background = 1;
		return;
	}
	if(lexemes[1] && !strcmp(lexemes[1],"&")){
		int i = 0;
		while(lexemes[i] && lexemes[i+1]){
			if(i >= 1){
				free(lexemes[i]);
				lexemes[i] = malloc(sizeof(char *) * strlen(lexemes[i+1]));
				strcpy(lexemes[i],lexemes[i+1]);
			}
			i++;
		}
		free(lexemes[i]);
		lexemes[i] = NULL;
		c->background = 1;
		return;
	}
}
//Cette fonction va parser la commande sous forme de lexeme est populer les redirections de la command_t c si necessaire sinon elle sont a NULL 
void handleRedirection(char ** commandLexemes,command_t * c){
	c->stdin_ptr = NULL;
	c->stdout_ptr = NULL;
	c->stderr_ptr = NULL;
	while(*commandLexemes){
		if(isRedirection(*commandLexemes)){
			if(isInRedirection(*commandLexemes)){
				commandLexemes++;
				if(fileExist(*commandLexemes)){
					c->stdin_ptr = malloc(sizeof(int *));
					*(c->stdin_ptr) = open(*commandLexemes,O_RDONLY);
				}
			}
			if(isOutRedirection(*commandLexemes)){
				if(!strcmp(*commandLexemes,">>") || !strcmp(*commandLexemes,"1>>")){
					commandLexemes++;
					if(fileExist(*commandLexemes)){
						c->stdout_ptr = malloc(sizeof(int *));
						*(c->stdout_ptr) = open(*commandLexemes,O_APPEND);
					}
				}else{
					commandLexemes++;
					c->stdout_ptr = malloc(sizeof(int *));
					*(c->stdout_ptr) = open(*commandLexemes,O_WRONLY);
				}
			}
			if(isErrRedirection(*commandLexemes)){
				if(!strcmp(*commandLexemes,"2>>") || !strcmp(*commandLexemes,"2>")){
					commandLexemes++;
					if(fileExist(*commandLexemes)){
						c->stderr_ptr = malloc(sizeof(int *));
						*(c->stderr_ptr) = open(*commandLexemes,O_APPEND);
					}
				}else{
					commandLexemes++;
					c->stderr_ptr = malloc(sizeof(int *));
					*(c->stderr_ptr) = open(*commandLexemes,O_WRONLY);
				}
			}
		}
		commandLexemes++;
	}
}
//Si le retour de la commande doit etre inverser on le note
void handleNot(char ** commandLexemes, command_t * c){
	if(!strcmp(*commandLexemes,"!")){
		c->invert = 1;
	}else{
		c->invert = 0;
	}
}
//Transforme une chaine de lexemes  en command_t
command_t * commandLexemesToCommand(char ** commandLexemes){
	command_t * c = malloc(sizeof(command_t));
	memset(c,0,sizeof(command_t));
	handlePipe(commandLexemes,c);
	int condition = whichCondition(commandLexemes[0]);
	if(condition){
		commandLexemes++;
	}
	handleNot(commandLexemes,c);
	if(c->invert){
		commandLexemes++;
	}
	handleBackgrounded(commandLexemes,c);
	handleRedirection(commandLexemes,c);
	char * path = findPath(commandLexemes[0]);
	char ** argv = extractArgv(commandLexemes);
	c->condition = condition;
	if(path != NULL){
		c->path = path;
	}
	if(argv != NULL){
		c->argv = argv;
	}

	return c;
}

void printCommand(command_t * c){
	#pragma GCC diagnostic ignored "-Wformat"
	printf("==============  %u  ===============\n", c);
	printf("|  Command to be executed -> %s    |\n", c->path);
	printf("|        condition -> %d                 |\n", c->condition);
	printf("=========================================\n");
}
//A partir d'une liste de lexemes de commande on va appaeler la fonction precedent pour remplir le tableau de commande
command_t ** commandLexemesListToCommandList(char *** commandLexemes){
	command_t ** commandList = malloc(sizeof(command_t **) * 50);
	memset(commandList,0,sizeof(command_t **) * 50);
	command_t ** ptr = commandList;
	while(*commandLexemes){
		*ptr = commandLexemesToCommand(*commandLexemes);
		//printCommand(*ptr);
		commandLexemes++;
		ptr++;
	}
	return commandList;
}
int isBuiltInCommand(char * commandName){
	int r = 0;
	r = r || !strcmp(commandName,"cd");
	r = r || !strcmp(commandName,"exit");
	return r;
}

// Permet d'executer une commande
void executeCommand(command_t * c, int * stop){
	signal(SIGINT,sigHandlerInt);
	signal(SIGTSTP,sigHandlerStp);
	if(c->path == NULL){
		if(isBuiltInCommand(*(c->argv))){
			if(!strcmp(*(c->argv),"exit")){
				_exit(0);
			}
			if(!strcmp(*(c->argv),"cd")){
				(c->argv)++;
				if(!chdir(*(c->argv))){
					c->status = 0;
				}else{
					c->status = 1;
				}
				(c->argv)--;
				free(c->argv);
				return;
			}
		}else{
			*stop = 1;
			printf("La commmande est [%s] introuvable\n", *(c->argv));
			return;			
		}
	}
	pid_t pid = fork();
	if(!pid){
		if(c->stdin_ptr != NULL){
			dup2(*(c->stdin_ptr),0);
		}
		if(c->stdout_ptr != NULL){
			dup2(*(c->stdout_ptr),1);
		}
		if(c->stderr_ptr != NULL){
			dup2(*(c->stderr_ptr),2);
		}
		execv(c->path,c->argv);
		printf("FAIL\n");
		_exit(EXIT_FAILURE);
	}else{
		runningProcess = pid;
		waitpid(pid,&(c->status), WUNTRACED | WCONTINUED);
	}
}
// Liberation de toutes la memoire alloué a une commmand_t
void freeCommand(command_t * c){
	if(c->path != NULL){
		free((c)->path);		
		free((c)->argv);
	}
	if((c)->stdin_ptr != NULL ){
		close(*((c)->stdin_ptr));
		if(!c->piped_in){
		 free((c)->stdin_ptr);
		}
	}
	if((c)->stdout_ptr != NULL){
		close(*((c)->stdout_ptr));
		if(!c->piped_out){
		 free((c)->stdout_ptr);
		}
	}
	if((c)->stderr_ptr != NULL){
		close(*((c)->stderr_ptr));
		free((c)->stderr_ptr);
	}
	free(c);
}
//Recupere un tableau de commande afin de les executer et a chaque execution etudie s'il faut executer les suivantes
void executeCommandList(command_t ** commandList){
	int stop = 0;
	int lastStatus = 0;
	while(*commandList && !stop){
		if(commandList[1] && (*commandList)->piped_out){
			int fd[2];
			memset(fd,0,sizeof(int*) * 2);
			pipe(fd);
			(*commandList)->stdout_ptr = &fd[1];
			commandList[1]->piped_in = 1;
			commandList[1]->stdin_ptr = &fd[0];
		}
		if((*commandList)->condition == 0 || (*commandList)->condition == 1){
			executeCommand(*commandList,&stop);
		}else{
			if((*commandList)->condition == 2){
				if(!lastStatus){
					executeCommand(*commandList,&stop);
				}else{
					stop = 1;
				}
			}else{
				if(lastStatus){
					executeCommand(*commandList,&stop);
				}else{
					stop = 1;
				}
			}
		}
		if((*commandList)->invert){
			lastStatus = !(*commandList)->status;
		}else{
			lastStatus = (*commandList)->status;	
		}
		freeCommand(*commandList);
		commandList++;
	}

}