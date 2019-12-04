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
#include <pthread.h>

#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"
#include "FN_COLADEESPERA.h"

#define COMMAND_LEN 140
#define LINEA_LEN 1024
#define DELETED -1

//pthread_mutex_t mutex;

char *requestCommand(ST_ESTACION *estacion){
    char *command = (char*)malloc(sizeof(char)*COMMAND_LEN+1);
    memset(command,'\0',COMMAND_LEN+1);
    printf("--------------------------------------------------------------------\n");
    printf("%s@ : " ,estacion->nombreEstacion);
    __fpurge(stdin);
    gets(command);
    
    return command;
}

bool dictionary (char *command){ //agregar comando solicitarAnden <nomTren> <estacionDestino>
    return strcmp(command,"estadoTren")==0 || strcmp(command,"partir")==0 || strcmp(command,"exit") ||
         strcmp(command,"estadoEstacion")==0 || strcmp(command,"eliminar")==0 ||strcmp(command,"limpiar")==0;
}

char *msgCat(const char *cad1, const char *cad2, const char*cad3, const char *cad4){
    char *msj = (char*)malloc(sizeof(char)*80);
    memset(msj, '\0', 80);
    msj = strcat(msj, cad1);
    msj = strcat(msj, cad2);
    msj = strcat(msj, cad3);
    msj = strcat(msj, cad4);
    msj = strcat(msj, "\n");
    return msj;
}

void showEstacion(ST_ESTACION *estacion){
    printf("Nombre de la estacion : %s\n", estacion->nombreEstacion);
    if(estacion->ocupado == false){
        printf("Estado del Anden : VACIO\n");
    }
    else{
        printf("Estado del Anden : \n");
        showTren(&estacion->ocupaAnden);
    }
    showQueue(estacion->colaDeEspera);
    printf("\n");
}

ST_TREN *searchTren (ST_ESTACION *estacion, char *nomTren){
    
    if(estacion->ocupado == true){
        if(strcmp(estacion->ocupaAnden.modelo, nomTren)==0){
            return &estacion->ocupaAnden;
        }
        else{
            return NULL;
        }
    }      
    else{
        ST_TREN *tren = search(estacion->colaDeEspera, nomTren);
        return tren;
    }
    return NULL;
}

int deleteTren(ST_ESTACION *estacion,char *nomTren){
    if(strcmp(estacion->ocupaAnden.modelo, nomTren)==0){
        estacion->ocupado = false;
        killProcess(estacion->ocupaAnden.pID);
    }
    else{
        ST_TREN *tren = delete(&estacion->colaDeEspera, nomTren);
        if(tren == NULL){
            return 1;
        }
        printf("TREN ELIMINADO\n");
        showTren(tren);
        killProcess(tren->pID);
    }
    
   return 0;
}

char *converChar (ST_TREN *tren){
    
    char *cad = (char*)malloc(sizeof(char) * COMMAND_LEN + 1);
    memset(cad, '\0', COMMAND_LEN + 1);
    printf(":C\n");
    
    sprintf(cad, "%s %d %d %d %s %s", tren->modelo, tren->infoTren.idTren, tren->combustible,
    tren->tiempoEspera, tren->infoTren.estacionOrigen, tren->infoTren.estacionDestino);
    
    printf("CAD %s\n", cad);
    return cad;
}

char * searchTrenToSend (ST_ESTACION *estacion, char *estacionDestino){
    char *trenLinea = (char*)malloc(sizeof(char) * LINEA_LEN + 1);
    ST_TREN *tren = searchTren(estacion, estacionDestino);
    if(tren == NULL){
        return NULL;
    }
    strcpy(tren->infoTren.estacionDestino, estacionDestino);
    strcpy(tren->infoTren.estacionDestino, estacion->nombreEstacion);
    
    trenLinea = converToChar(tren, tren->infoTren.motivo);
    
    return trenLinea;
}

void partirTren(ST_ESTACION *estacion, char *command, int new_socket ){
  
    char *nomTren = strtok(NULL , " ");
    char *estacionDestino = strtok(NULL," ");
    char *trenLinea = searchTrenToSend(estacion, nomTren);
    
    if(trenLinea == NULL){
        printf("ERROR TREN NO ENCONTRADO!\n");
        return;
    }
    
    char *ip = getIP(estacionDestino);
 
    int port = getPort(estacionDestino);
    
    printf("IP %s\n", ip);
    printf("PORT %d\n", port);
    printf("BUFFER %s\n", trenLinea);
  
    cliente(trenLinea, port, ip);
    
    printf("CLIENTE OK\n");
    
    sendMsg(new_socket, msgCat("El ", nomTren, " reside en la " , estacionDestino));
    
    deleteTren(estacion, nomTren);
}

void commandList(){
    printf("--------------------------------------------------------------------\n\t\t\tCOMANDOS PERMITIDOS :\n");
    printf("estadoTren <nombreTren>, eliminar <nombreTren>\npartir <nombreTren> <nombreEstacion>\nestadoEstacion, ayuda, limpiar, salir\n");
}

void estadoTren (ST_ESTACION *estacion, char *nomTren){
  
    ST_TREN *tren = searchTren(estacion, nomTren);
    
    if(tren == NULL){
        printf("TREN NO ENCONTRADO\n");
    }
    else{
        printf("Modelo : %s\n ID del Tren : %d\n Combustible : %d m\n Tiempo de Espera : %d L\n Estacion Origen : %s\n Estacion Destino : %s\n",
        tren->modelo,tren->infoTren.idTren,tren->combustible,tren->tiempoEspera,
        tren->infoTren.estacionOrigen,tren->infoTren.estacionDestino);   
    }
}

void processCommand(ST_ESTACION *estacion, char *command, int new_socket){
    
    char *action = strtok(command, " ");// saca la accion
    
    
    if(strcmp(action, "estadoEstacion")==0){
        showEstacion(estacion);
    }
    
    if(strcmp(action, "partir")==0){
        partirTren(estacion, command, new_socket);
    }
    
    if(strcmp(action, "ayuda")==0){
        commandList();
    }
    
    if(strcmp(action, "eliminar")==0){      
        if(deleteTren(estacion, strtok(NULL, " "))==0){// estacion , nomTren
            printf("Se elimino exitosamente!\n");
        }
        else{
            printf("TREN NO ENCONTRADO!\n");
        }
    }
    
    if(strcmp(action,"estadoTren")==0){
        estadoTren(estacion, strtok(NULL, " "));//estacion, nomTren
    }
    
    if(strcmp(action, "limpiar")==0){
        system("clear");
    }
    if(strcmp(action, "solicitarAnden")==0){
        //solicitarAnden(estacion);
    }
}

void *commandEstacion(void *station){
    ST_ESTACION *estacion = station;
 
    char *command = requestCommand(estacion);
    
    //pthread_mutex_lock(&mutex);
    
    while(strcmp(command, "salir")!=0){
        while(!dictionary(command)){
            printf("comando desconocido\n");
            command = requestCommand(estacion);
        }
        processCommand(estacion, command, estacion->new_socket);
        command = requestCommand(estacion);    
    }
    
    //pthread_mutex_unlock(&mutex);
    
    //return NULL;
}