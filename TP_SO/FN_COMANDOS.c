#include <stdio.h>
#include <string.h>  
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>
#include <unistd.h>   
#include <arpa/inet.h>  
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <stdio_ext.h>//__purge

#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"

#define COMMAND_LEN 80
#define MAX_CLIENTS 5


char *requestCommand(ST_ESTACION *estacion){
    char *command = (char*)malloc(sizeof(char)*COMMAND_LEN+1);
    memset(command,'\0',COMMAND_LEN+1);
    printf("---------------------------------------------------\n");
    printf("%s@ : ",estacion->nombreEstacion);
    __fpurge(stdin);
    gets(command);
    return command;
}

bool dictionary (char *command){
    return strcmp(command,"estadoTren")==0 || strcmp(command,"partir")==0 || strcmp(command,"exit") ||
         strcmp(command,"estadoEstacion")==0 || strcmp(command,"eliminar")==0;
}

char *msgCat(const char *cad1,const char *cad2,const char*cad3,const char *cad4){
    char *msj = (char*)malloc(sizeof(char)*80);
    memset(msj,'\0',80);
    msj = strcat(msj,cad1);
    msj = strcat(msj,cad2);
    msj = strcat(msj,cad3);
    msj = strcat(msj,cad4);
    msj = strcat(msj,"\n");//->prueba
    return msj;
}

void showEstacion(ST_ESTACION *estacion){
    printf("---------------------------------------------------------\n");
    printf("Nombre de la estacion : %s\n",estacion->nombreEstacion);
    if(estacion->ocupado == false){
        printf("Estado del Anden : VACIO\n");
    }
    else{
        printf("Estado del Anden : %s\n",estacion->ocupaAnden.modelo);
    }
    showQueue(estacion);
    printf("\n");
}

void partirTren(ST_ESTACION *estacion,char *command,int new_socket){
    char *path = (char*)malloc(sizeof(char)*10);
    char *estacionDestino = (char*)malloc(sizeof(char)*20);
    char *auxTren = (char*)malloc(sizeof(char)*200);
    char *linea = (char*)malloc(sizeof(char)*50);
    memset(auxTren,'\0',200);
    
    printf("%s ",command);
    __fpurge(stdin);
    gets(linea);
    
    sscanf(linea,"%s %s",path,estacionDestino);
    
    char *datosTren = readFileConfig(path);
    printf("DATOS DEL TREN : %s\n",datosTren);
    
    char *ip = getIP(estacionDestino);
    printf("IP %s\n",ip);
    
    int port = getPort(estacionDestino);
    printf("PUERTO %d\n",port);
    
    strcpy(command,"reenviado ");
    auxTren = strcat(command,datosTren);
  
    cliente(auxTren,port,ip);
    
    sendMsg(new_socket,msgCat("El tren ",path, " reside en la " ,estacionDestino));//:C
}

void commandList(){
    printf("estadoTren, eliminar, salir, partir, estadoEstacion, ayuda\n");
}


int deleteTren(ST_ESTACION *estacion){
    showEstacion(estacion);
    char nomTren[10];
    printf("eliminar ");
    scanf("%s",nomTren);
    if(strcmp(estacion->ocupaAnden.modelo,nomTren)==0){
        estacion->ocupado = false;
        return 0;
    }else{
        for(int i=0;i<MAX_CLIENTS;i++){
           if(strcmp(estacion->colaDeEspera[i].modelo,nomTren)==0){
               estacion->colaDeEspera[i].infoTren.idTren = -1;
               return 0;
           }
        }
    }
    return 1;
}

ST_TREN *searchTren (ST_ESTACION *estacion,char *nomTren){
    ST_TREN *tren = (ST_TREN*)malloc(sizeof(ST_TREN));
    if(strcmp(estacion->ocupaAnden.modelo,nomTren)==0){
        tren = &estacion->ocupaAnden;
        return tren;
    }else{
        for(int i=0;i<MAX_CLIENTS;i++){
            if(strcmp(estacion->colaDeEspera[i].modelo,nomTren)==0){
                tren = &estacion->colaDeEspera[i];
                return tren;
            }
        }
    }
    return NULL;
}

void estadoTren (ST_ESTACION *estacion){
    char nomTren[10];
    printf("Estado del ");
    scanf("%s",nomTren);
    ST_TREN *tren = searchTren(estacion,nomTren);
    if(tren != NULL){
        printf("Modelo : %s\n ID del Tren : %d\n Tiempo de Espera : %d m\n Combustible : %d L\n Estacion Origen : %s\n Estacion Destino : %s\n",
        tren->modelo,tren->infoTren.idTren,tren->combustible,tren->tiempoEspera,
        tren->infoTren.estacionOrigen,tren->infoTren.estacionDestino);
    }
    else{
        printf("TREN NO ENCONTRADO\n");
    }
}

void processCommand(ST_ESTACION *estacion,char *command,int new_socket){
    if(strcmp(command,"estadoEstacion")==0){
        showEstacion(estacion);
    }
    if(strcmp(command,"partir")==0){
        partirTren(estacion,command,new_socket);
    }
    if(strcmp(command,"ayuda")==0){
        commandList();
    }
    if(strcmp(command,"eliminar")==0){
        if(deleteTren(estacion)==0){
            printf("Se elimino exitosamente!\n");
        }
        else{
            printf("Tren no encontrado!\n");
        }
    }
    if(strcmp(command,"estadoTren")==0){
        estadoTren(estacion);
    }
    //Si es salir no hace nada
}

char *commandEstacion(ST_ESTACION *estacion,int new_socket){
    char *command = requestCommand(estacion);// estadoEstacion, partir <nombre del tren> (pregunta por el anden si esta libre, sino lo esta se inserta en la cola de espera 
    while(!dictionary(command)){
        printf("comando desconocido\n");
        command = requestCommand(estacion);
    }
    processCommand(estacion,command,new_socket);
    
    return command;
}
