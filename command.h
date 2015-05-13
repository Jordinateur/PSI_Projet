#ifndef COMMAND
#define COMMAND

typedef struct command {
	char * path;
	char ** argv;
	int * stdin_ptr;
	int * stdout_ptr;
	int * stderr_ptr;
	int background;
	int status;
	int condition;
	int invert;
	int piped_in;
	int piped_out;
} command_t;

int fileExist(char * fName);
int isCommandDelim(char * lexeme);
int whichCondition(char * lexeme);
int isInRedirection(char * lexeme);
int isOutRedirection(char * lexeme);
int isErrRedirection(char * lexeme);
int isRedirection(char * lexeme);
char ** extractArgv(char ** lexemes);
char * findPath(char * lexeme);
int redirectToTrash();
void handleBackgrounded(char ** lexemes, command_t * c);
void handleRedirection(char ** commandLexemes,command_t * c);
command_t * commandLexemesToCommand(char ** commandLexemes);
void printCommand(command_t * c);
command_t ** commandLexemesListToCommandList(char *** commandLexemes);
void executeCommand(command_t * c, int * stop);
void freeCommand(command_t * c);
void executeCommandList(command_t ** commandList);
#endif