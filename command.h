#ifndef COMMMAND
#define COMMMAND 4
typedef struct command {
	char * path;
	char ** argv;
	void * stdin_ptr;
	void * stdout_ptr;
	void * stderr_ptr;
	int background;
	int status;
} command;

command * lexemesToCommand(char ** lexemes);
#endif