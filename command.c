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
	while(*(c->argv)){
		printf("%s\n", *(c->argv));
		c->argv++;
	}
	return c;

}
//Retourne le chemin d'une commande si elle existe dans le PATH
char * findPathForCommand(char * command){
	char * full_path = getenv("PATH");
	char * path = NULL;
	char * test = NULL;
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