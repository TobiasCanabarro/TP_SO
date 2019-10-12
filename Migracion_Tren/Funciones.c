#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <stdio_ext.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Estructuras.h"
#include "Funciones.h"

#define IPaConectar "127.0.0.1"
#define LONG_COMANDO 1024
#define LONGTREN 1024
#define PUERTO 7400

void discardChars(){
    char c;
    while((c = getchar()) != '\n' && c != EOF);
    return;
}

char *creaTren(){
    char *tren = (char*)malloc(sizeof(char)*LONGTREN+1);
    memset(tren,'\0',LONGTREN+1);
    printf("Modelo de tren e ID :");
    __fpurge(stdin);
    int trenLen = strlen(tren);
    getline(&tren,(void*)&trenLen,stdin);
    //gets(tren);
    return tren;
}

char * pideComando(){
    char *comando = (char*)malloc(sizeof(char)*LONG_COMANDO+1);
    memset(comando,'\0',LONG_COMANDO+1);
    printf("usr@-pc : ");
    __fpurge(stdin);
    //int comandoLen = strlen(comando);
    //getline(&comando,(void*)&comandoLen,stdin);
    gets(comando);
    return comando;
}

char * sacoPath(char *linea){
    char *aux = linea;
    while(*aux && *aux != ' '){
        aux++;
    }
    aux++;
    return aux;
}

char *sacoEstacion(char *comando,int cant){
    char *aux = comando;
    char *token = strtok(aux," ");
    while(token != NULL && cant > 0){
       token = strtok(NULL," ");
       cant --;
    }
    return aux;
}

FILE *modeOpenFile(const char *path,const char *mode){
    FILE *pFile = NULL;
    pFile = fopen(path,mode);
    if(pFile == NULL){
        printf("Error al leer el archivo configuracion del tren\n");
        exit(EXIT_FAILURE);
    }
    return pFile ;
}

char *leeFileConfig(char *path){
    FILE *pFile = modeOpenFile(path,"r");
    char *linea = (char*)malloc(sizeof(char)*40);
    memset(linea,'\0',40);
    fgets(linea,40,pFile);
    return linea;
}

char *registrar (char *comando,char *path){
    char *tren = leeFileConfig(path);

    return tren;
}

char *acciones (char* accion,char *comando){
    char * tren = (char*)malloc(sizeof(char)*LONGTREN+1);

    char *auxComando = comando;
    char *path = sacoPath(auxComando);

    if(strcmp(accion,"solicitar")==0){

    }

    if(strcmp(accion,"partir")==0){
        //partir(comando);
    }

    if(strcmp(accion,"registrar")==0){
        strcpy(tren , registrar(comando,path));
        return tren;
    }

    if(strcmp(accion,"estado")==0){

    }

    if(strcmp(accion,"exit")==0){
        return "exit";
    }
    return "desconocido";
}

char *sacoAccion(char *cad,char separador){
    char *auxCad = cad;
    char *acumCad = (char*)malloc(sizeof(char)*LONG_COMANDO+1);
    char *auxAcum = acumCad;

    while(*auxCad && *auxCad != separador){
        *auxAcum = *auxCad;
        auxAcum++;
        auxCad++;
    }
    *auxAcum = '\0';
    return acumCad;
}

char *comando(){
    char *linea = pideComando();
    char *auxLinea = linea;
    char *accion = sacoAccion(auxLinea,' ');

    char *tren = acciones(accion,linea);

    return tren;
}

int cliente(char *tren){
        int idSocket;
        int activado;
        char msj [20];
        memset(msj,'\0',20);
        struct sockaddr_in direccionServidor;
        direccionServidor.sin_family = AF_INET;
        direccionServidor.sin_addr.s_addr = inet_addr(IPaConectar);//IP a conectar
        direccionServidor.sin_port = htons(PUERTO);//Puerto a conectar

        idSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(idSocket < 0){
            printf("No se pudo conseguir el socket\n");
        }

        activado = 1;
        setsockopt(idSocket, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

        if(connect(idSocket, (void*) &direccionServidor, sizeof(direccionServidor)) != 0){
            perror("No se pudo conectar al servidor\n.");
        return 1;
        }

        send(idSocket,tren,strlen(tren),0);

        if(recv(idSocket,msj,30,0) < 0){
            printf("Error al enviar el tren %s.\n",tren);
        }

        printf("%s",msj);
        close(idSocket);
        printf("-------------------------------------------------------------------------------\n");
        return 0;
}


