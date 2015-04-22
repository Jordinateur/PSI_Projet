#ifndef SHELL_UTILS
#define SHELL_UTILS 2
typedef struct command {
	char * path;
	char ** argv;
	void * stdin_ptr;
	void * stdout_ptr;
	void * stderr_ptr;
	int background;
	int status;
} command;

char * getLine(char* rawInput);
char ** strToLexeme(char * str);
int isRedirection(char * lexeme);
char * findPathForCommand(char * command);
command * lexemesToCommand(char ** lexemes);
#endif
