#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
//Ajoute un caractere a une chaine de caractere et retourne la chaine cree
char* appendCharToString(char* array, char a)
{
    char* out;
	if(!array){
        printf(" !array /!\\ \n");
        out = malloc(sizeof(char *) * 2);
        out[0] = a;
        out[1] = '\0';
		return out;
	}
    size_t len = strlen(array);
    out = malloc(sizeof(char *)  * (len+2));

    strcpy(out, array);
    out[len] = a;
    out[len+1] = '\0';

    return out;
}
char* appendCharToString2(char* out, char a)
{
    if(!out){
        out = malloc(sizeof(char *) * 2);
        out[0] = a;
        out[1] = '\0';
        return out;
    }
    size_t len = strlen(out);
    out = realloc(out,sizeof(char *)  * (len+2));
    strcpy(out, out);
    out[len] = a;
    out[len+1] = '\0';

    return out;
}
//Ajoute une chaine de caractere a une liste de chaine de caractere
char ** appendStringToStringList(char ** strList, char* str){
    char** out;
    int count = 0;
    char ** ptr = strList;
    if(!ptr){
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
char ** appendStringToStringList2(char ** strList, char* str){
    int count = 0;
    char ** ptr = strList;
    if(!ptr){
        strList = malloc(sizeof(char**) * 2);
        strList[0] = str;
        strList[1] = NULL;
        return strList;
    }
    while(*ptr){
        ptr++;
        count++;
    }
    strList = realloc(strList,sizeof(char**) * (count + 2));
    memcpy(strList,strList,sizeof(char**) * count);
    strList[count] = str;
    strList[count + 1] = NULL;
    return strList;
}
//Teste si un fichier existe
int fileExist(char * fpath){
    if( access( fpath, F_OK ) != -1 ) {
        return 1;
    } else {
        return 0;
    }
}