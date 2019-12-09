/* 
 * File:   FUNCIONES.c
 * Author: tobias-pc
 * 
 * Created on 27 de noviembre de 2019, 11:43
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio_ext.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "FUNCIONES.h"
#include "ESTRUCTURAS.h"

#define ipEstacionA "127.0.0.1"
#define ipEstacionB "127.0.0.1"//"192.168.1.16"
#define ipEstacionC "127.0.0.1"

#define LONG_COMANDO 1024
#define LONGTREN 1024
#define LONGMSJ 80


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

bool conoceComando(char *accion){
    return strcmp(accion,"registrar")==0 || strcmp(accion,"solicitarAnden")==0 ||strcmp(accion,"exit")==0;
}

ST_TREN *convToStruct(char *cadTren, const char *formato ){
    ST_TREN *tren = (ST_TREN*)malloc(sizeof(ST_TREN));
    char *aux =(char*)malloc(sizeof(char)*20);// 
    
    sscanf(cadTren, formato, aux, tren->modelo,
    &tren->infoTren.idTren, &tren->combustible,
    &tren->tiempoEspera, tren->infoTren.estacionOrigen, tren->infoTren.estacionDestino);
    
    free(aux);
    return tren;
}

char *obtieneIP(char *nomEstacion){
    
    if(strcmp(nomEstacion, "estacionA")==0){       
        return ipEstacionA;
    }
    
    if(strcmp(nomEstacion, "estacionB")==0){
        return ipEstacionB;
    }
    
    return ipEstacionC;
}

char *obtieneEstacion (char *path){
    printf("%s\n",path);
    char *cadTren = leeFileConfig(path);
    printf("%s\n",cadTren);
    ST_TREN *tren = (ST_TREN*)malloc(sizeof(ST_TREN));
    sscanf(cadTren, "%s %d %d %d %s %s", tren->modelo,
    &tren->infoTren.idTren, &tren->tiempoEspera,
    &tren->combustible, tren->infoTren.estacionOrigen, tren->infoTren.estacionDestino);
    printf("%s\n",tren->infoTren.estacionOrigen);
    
    return tren->infoTren.estacionOrigen;
}

char *obtienePuerto(char *argv){
    
    char *nomEstacion = obtieneEstacion(argv);
    
    if(strcmp(nomEstacion, "estacionA")==0){
        return "7400";
    }
    if(strcmp(nomEstacion, "estacionB")==0){    
        return "7800";
    }
   
    return "8200";
}
                                             //   0        1          2         
char *parceoArgv(char *tren, char *argv[]){// <./tren> <accion> <path/nomTren> 
    
    if(!conoceComando(argv[1])){
        return NULL;
    }
    
    strcpy(tren, argv[1]);
    
    tren  = strcat(tren, " ");
    
    char *aux = leeFileConfig(argv[2]);
   
    tren = strcat(tren, aux);
 
    return tren;
}

char *procesoArgumentos (char *argv[]){
    char *tren = (char*)malloc(sizeof(char) * LONGTREN + 1);
    
    if(parceoArgv(tren ,argv) == NULL){
        return NULL;
    }
    
    return tren;
}

char *creaTren(char *argv[]){
     char *tren = procesoArgumentos(argv);
    
    if(tren == NULL){
        printf("ACCION DESCONOCIDA\n");
        exit(EXIT_FAILURE);
    }
    return tren;
}

void trenCreado(char *newTren){
    printf("------------------------------ Tren Creado ------------------------------\n");
    ST_TREN *tren = convToStruct(newTren, "%s %s %d %d %d %s %s");
    
    printf("Modelo : %s\n ID del Tren : %d\n Combustible : %d L\n Tiempo de Espera : %d m\n Estacion Origen : %s\n Estacion Destino : %s\n",
    tren->modelo,tren->infoTren.idTren,tren->combustible,tren->tiempoEspera,
    tren->infoTren.estacionOrigen,tren->infoTren.estacionDestino);
    printf("-------------------------------------------------------------------------\n");
}

int cliente(char *tren,char *puerto,char *ip){
        int idSocket;
        int activado;
        char msj [LONGMSJ+1];
        
        memset(msj,'\0',LONGMSJ+1);
        
        struct sockaddr_in direccionServidor;
        direccionServidor.sin_family = AF_INET;
        direccionServidor.sin_addr.s_addr = inet_addr(ip);
        direccionServidor.sin_port = htons(atoi(puerto));

        idSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(idSocket < 0){
            printf("NO SE PUDO CONSEGUIR EL SOCKET\n");
        }

        activado = 1;
        setsockopt(idSocket, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

        if(connect(idSocket, (void*) &direccionServidor, sizeof(direccionServidor)) != 0){
            perror("SERVIDOR DESCONECTADO\n.");
            return 1;
        }

        send(idSocket,tren,strlen(tren),0);

        if(recv(idSocket,msj,LONGMSJ,0) < 0){
            printf("ERROR AL ENVIAR EL %s.\n", tren);
        }

        printf("%s",msj);
        close(idSocket);
        printf("-------------------------------------------------------------------------------\n");
        return 0;
}

void onLine(bool flag){
    while(flag){
        
    }
}
