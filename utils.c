#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
//Ajoute un caractere a une chaine de caractere et retourne la chaine cree
char* appendCharToString(char* array, char a)
{
    char* out;
	if(array == NULL || !array){
        out = (char *) malloc(2);
        out[0] = a;
        out[1] = '\0';
		return out;
	}
    size_t len = strlen(array);
    out = (char *) malloc(len+2);

    strcpy(out, array);    
    out[len] = a;
    out[len+1] = '\0';

    return out;
}
//Ajoute une chaine de caractere a une liste de chaine de caractere
char ** appendStringToStringList(char ** strList, char* str){
    char** out;
    int count = 0;
    char ** ptr = strList;
    if(!strList){
        out = malloc(sizeof(char**) * 2);
        out[0] = str;
        out[1] = NULL;
        return out;
    }
    while(*ptr != NULL){
        ptr++;
        count++;
    }
    out = malloc(sizeof(char**) * (count + 2));
    memcpy(out,strList,sizeof(char**) * count);
    out[count] = str;
    out[count + 1] = NULL;
    return out;
}
//Teste si un fichier existe
int fileExist(char * fpath){
    if( access( fpath, F_OK ) != -1 ) {
        return 1;
    } else {
        return 0;
    }
}