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
} command;
int isRedirection(char * lexeme);
char * findPathForCommand(char * command);
command * lexemesToCommand(char ** lexemes);
char ** extractArgv(char ** lexemes);
int isBackgrounded(char* binName);
pid_t executeCommand(command * c);
#endif