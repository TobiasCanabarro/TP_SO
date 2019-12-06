
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define PORT 7400
#define MAXBUFFER 1024
#define LONG_COMANDO 1024

#define ipEstacionA "127.0.0.1"//"127.0.0.1"
#define ipEstacionB "127.0.0.1"//"192.168.1.16" <- ip rober
#define ipEstacionC "127.0.0.1"

#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"

FILE *modeOpenFile(char *path,char *mode){
    FILE *pFile = NULL;
    pFile = fopen(path,mode);
    if(pFile == NULL){
        printf("ERROR AL LEER EL ARCHIVO DE CONFIGURACION\n");
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

char *myStrtok(char *linea,char separador){
    char *acum = (char*)malloc(sizeof(char)*20);
    char *aux = acum;
    while(*linea && *linea != separador){
        *aux = *linea;
        linea++;
        aux++;
    }
    *aux = '\0';
    return acum;
}

char *readFileConfig(char *path){
    FILE *pFile = modeOpenFile(path,"r");
    char *linea = (char*)malloc(sizeof(char)*80);
    memset(linea,'\0',80);
    fgets(linea,80,pFile); 
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

char *getIP (char *nomEstacion){
    if(strcmp(nomEstacion,"estacionA")==0){
        return ipEstacionA;
    }
    if(strcmp(nomEstacion,"estacionB")==0){
        return ipEstacionB;
    }
    return ipEstacionC;
}

int cliente(char *tren,int puerto,char *ip){
        int idSocket;
        int activado;
        char msj [50];
        memset(msj,'\0',50);
        struct sockaddr_in direccionServidor;
        direccionServidor.sin_family = AF_INET;
        direccionServidor.sin_addr.s_addr = inet_addr(ip);//IP a conectar
        direccionServidor.sin_port = htons(puerto);//Puerto a conectar

        idSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(idSocket < 0){
            printf("NO SE PUDO CONSEGUIR EL SOCKET\n");
        }

        activado = 1;
        setsockopt(idSocket, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

        if(connect(idSocket, (void*) &direccionServidor, sizeof(direccionServidor)) != 0){
            perror("NO SE PUDO CONECTAR AL SERVIDO\n.");
            return 1;
        }

        send(idSocket, tren, strlen(tren), 0);//send(idSocket, tren, strlen(tren), 0)
        printf("SEND OK\n");
        
        if(recv(idSocket,msj,50,0) < 0){
            printf("ERROR AL ENVIAR EL TREN %s.\n",tren);
        }
        printf("RECV OK\n");
        
        close(idSocket);
        printf("\n-------------------------------------------------------------------------------\n");
        return 0;
}