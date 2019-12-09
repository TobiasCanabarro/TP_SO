
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

#define ipEstacionA "127.0.0.1"
#define ipEstacionB "127.0.0.1"//"192.168.1.16" <- ip rober
#define ipEstacionC "127.0.0.1"
#define DELETED -1

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

int getPort(char *nomEstacion){
    
    if(strcmp(nomEstacion, "estacionA")==0){
        return 7400;
    }
    if(strcmp(nomEstacion, "estacionB")==0){
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

void showTren(ST_TREN *tren){
    if(tren->infoTren.idTren != DELETED){
        printf("\n");
        printf("Modelo : %s\n ID del Tren : %d\n Combustible : %d L\n Tiempo de Espera : %d m\n Estacion Origen : %s\n Estacion Destino : %s\n PID : %d\n Estado Del Tren : %s\n Motivo Del Tren : %s\n",
        tren->modelo, tren->infoTren.idTren, tren->combustible, tren->tiempoEspera,
        tren->infoTren.estacionOrigen, tren->infoTren.estacionDestino, tren->pID, tren->estado, tren->infoTren.motivo);
    }     
}

int cliente(char *tren,int puerto,char *ip){
        int activado;
        char buffer [MAXBUFFER + 1 ];
        memset(buffer, '\0', MAXBUFFER + 1);
        struct sockaddr_in direccionServidor;
        direccionServidor.sin_family = AF_INET;
        direccionServidor.sin_addr.s_addr = inet_addr(ip);
        direccionServidor.sin_port = htons(puerto);

        int idSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(idSocket < 0){
            printf("NO SE PUDO CONSEGUIR EL SOCKET\n");
            return 1;
        }

        activado = 1;
        setsockopt(idSocket, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

        if(connect(idSocket, (void*) &direccionServidor, sizeof(direccionServidor)) != 0){
            perror("NO SE PUDO CONECTAR AL SERVIDO\n.");
            return 1;
        }

        sendMsg (idSocket, tren);

        recvMsg (idSocket, buffer);
        close(idSocket);
        printf("\n-------------------------------------------------------------------------------\n");
        return 0;
}