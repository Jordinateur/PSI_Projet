#ifndef COMMMAND
#define COMMMAND
#include <sys/types.h>
typedef struct command {
	char * path;
	char ** argv;
	int stdin_ptr;
	int stdout_ptr;
	int stderr_ptr;
	int background;
	int status;
	int condition;
} command;
int isRedirection(char * lexeme);
char * findPathForCommand(char * command);
void lexemesToCommand(char ** lexemes, command * c);
char ** extractArgv(char ** lexemes);
int isBackgrounded(char* binName);
pid_t executeCommand(command * c);
void freeCommand(command * c);
#endif