#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define PORT 7400
#define MAXBUFFER 1024
#define LONG_COMANDO 1024


#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"

FILE *modeOpenFile(char *path,char *mode){
    FILE *pFile = NULL;
    pFile = fopen(path,mode);
    if(pFile == NULL){
        printf("Error al leer el archivo configuracion del tren\n");
        exit(EXIT_FAILURE);
    }
    return pFile ;
}

char * quitPath(char *linea){
    char *aux = linea;
    while(*aux && *aux != ' '){
        aux++;
    }
    aux++;
    return aux;
}


char *readFileConfig(char *path){
    FILE *pFile = modeOpenFile(path,"r");
    char *linea = (char*)malloc(sizeof(char)*20);
    memset(linea,'\0',20);
    fgets(linea,20,pFile); 
    fclose(pFile);
    return linea;
}


int getPort(char *nomEstacion){
    
    if(strcmp(nomEstacion,"estacionA")==0){
        return 7400;
    }
    if(strcmp(nomEstacion,"estacionB")==0){
        return 7800;
    }
    return 8200;
}