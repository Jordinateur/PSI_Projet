//Recupere le nom d'une variable et retourne sa valeur
char* replaceVar(char *varName){
	char *varValue = getenv(varName);
	if(varValue){
		return varValue;
	}else{
		return varName;
	}
}
//Retourne vrai si la chaine en entree est une variable
int isVarToReplace(char* string){
	if(string[0] == '$'){
		return 1;
	}else{
		return 0;
	}
}