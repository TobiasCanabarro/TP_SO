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

char * comando(){
    char *comando = (char*)malloc(sizeof(char)*LONG_COMANDO+1);
    memset(comando,'\0',LONG_COMANDO+1);
    printf("usr@-pc : ");
    __fpurge(stdin);
    gets(comando);
    return comando;
    
}

ST_TREN registrar (char *comando){
    ST_TREN tren;
    strtok(comando," ");
    strcpy(tren.modelo,strtok(NULL," "));
    tren.infoTren.idTren = atoi(strtok(NULL," "));
    return tren;
}

ST_TREN acciones (char* accion,char *comando){
    printf("%s %s\n",accion,comando);
    if(strcmp(accion,"solicitar")==0){
        
    }
    if(strcmp(accion,"partir")==0){
        
    }
    if(strcmp(accion,"registrar")){
        ST_TREN tren = registrar(comando);
        return tren;
    }
    if(strcmp(accion,"estado")==0){
   
    }
}

char *sacoAccion(char *cad,char separador){
    char *auxCad = cad;
    char *acumCad = (char*)malloc(sizeof(char)*LONG_COMANDO+1);
    char *auxAcum = acumCad;
    while(*auxCad){
        if(*auxCad == separador){
            *auxAcum = '\0';
            return acumCad;
        }
        *auxAcum = *auxCad;
        auxAcum++;
        auxCad++;
    }
    return NULL;
}

ST_TREN opciones (char *comando){
    char *aux = comando;
    char *opcion = sacoAccion(aux,' ');
  
   ST_TREN tren = acciones(opcion,comando);
   return tren;
}

