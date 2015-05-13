#ifndef SHELL_UTILS
#define SHELL_UTILS

char * getLine(char* rawInput);
char ** strToLexeme(char * str);
char ** strToLexemes(char * str);
int * splitPointLexemes(char ** lexemes);
char *** lexemesToCommands(char ** lexemes,int * splitPoints);
#endif
