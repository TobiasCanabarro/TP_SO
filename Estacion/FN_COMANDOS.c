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
#define MAX_SLEEP 1

//pthread_mutex_t mutex;

char *requestCommand(ST_ESTACION *estacion){
    char *command = (char*)malloc(sizeof(char)*COMMAND_LEN + 1);
    memset(command, '\0', COMMAND_LEN + 1);
    printf("--------------------------------------------------------------------\n");
    printf("%s@ : " , estacion->nombreEstacion);
    __fpurge(stdin);
    gets(command);
    
    return command;
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
    
    printf("--------------------------------------------------------------------\n");
    printf("\t\t\t-=%s=-\n", estacion->nombreEstacion);
    printf("--------------------------------------------------------------------\n");
    if(estacion->ocupado == false){
        printf("\t\tESTADO DEL ANDEN : VACIO\n");
    }
    else{
        printf("\t\tESTADO DEL ANDEN : \n");
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
        ST_TREN *tren = searchInQueue (estacion->colaDeEspera, nomTren);
        return tren;
    }
    return NULL;
}

int deleteTren(ST_ESTACION *estacion, char *nomTren){
    if(strcmp(estacion->ocupaAnden.modelo, nomTren)==0){
        estacion->ocupado = false;
        estacion->ocupaAnden.infoTren.idTren = DELETED;
        //killProcess(estacion->ocupaAnden.pID);
    }
    else{
        ST_TREN *tren = deleteTrenInQueue(&estacion->colaDeEspera, nomTren);
        if(tren == NULL){
            return 1;
        }
        //killProcess(tren->pID);
    }
   return 0;
}

int partirTren(ST_ESTACION *estacion, char *command, int new_socket ){
  
    char *nomTren = strtok(NULL , " ");
    char *estacionDestino = strtok(NULL, " ");

    if ( strcmp(estacion->ocupaAnden.modelo, nomTren) != 0 ){
        return 1;
    }
    strcpy(estacion->ocupaAnden.infoTren.estacionOrigen, estacion->nombreEstacion);
    strcpy(estacion->ocupaAnden.infoTren.estacionDestino, estacionDestino);

    char *trenLinea = converToChar ( &estacion->ocupaAnden, estacion->ocupaAnden.infoTren.motivo);

    if(trenLinea == NULL){//temporal
        printf("ERROR EN CONVERTOCHAR\n");
        return 2;
    }

    if(deleteTren(estacion, nomTren) == 1){
        return 2;
    }

    char *ip = getIP(estacionDestino);
 
    int port = getPort(estacionDestino);
    
    cliente(trenLinea, port, ip);
    
    printf("El %s partio a la %s\n",nomTren, estacionDestino);
    return 0;
}

void commandList(){
    printf("--------------------------------------------------------------------\n\t\t\tCOMANDOS PERMITIDOS :\n");
    printf("estadoTren <nombreTren>, eliminar <nombreTren>, solicitarAnden <nombreTren>\npartir <nombreTren> <nombreEstacion>\nestadoEstacion, ayuda, limpiar, actualizar, registroDeTrenes\n");
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

void showTrenReport (ST_ESTACION *estacion){
    char *path = (char*)malloc(sizeof(char)* COMMAND_LEN + 1);
    char aux[] = "registroDeTrenes";
    strcpy (path, aux);
    path = strcat (path, estacion->nombreEstacion);
    path = strcat (path, ".txt");
    FILE *pFile  = modeOpenFile(path ,"r");
    char linea [80];
     printf("--------------------------------------------------------------------\n\t\t\tREGISTRO DE TRENES :\n");
    while(fgets(linea, 80, pFile)){
        printf("%s", linea);
    }
    fclose (pFile);
}

bool processCommand(ST_ESTACION *estacion, char *command, int new_socket){
 
    char *action = strtok(command, " ");//partir TrenA estacionB
    
    if(strcmp(action, "estadoEstacion") == 0){
        showEstacion(estacion);
        return true;
    }
    
    if(strcmp(action, "partir") == 0){ 
        switch( partirTren (estacion, command, new_socket) ){
            case 1:
                printf("ERROR EL TREN NO SE ENCUENTRA EN EL ANDEN\n");
                break;
            case 2:
                printf("ERROR EL TREN NO SE ENCUENTRA EN LA ESTACION\n");\
                break;
        }                
        return true;
    }
    
    if(strcmp(action, "ayuda") == 0){
        commandList();
        return true;
    }
    
    if(strcmp(action, "eliminar") == 0){      
        if(deleteTren(estacion, strtok(NULL, " ")) == 0){
            printf("Se elimino exitosamente!\n");
        }
        else{
            printf("TREN NO ENCONTRADO!\n");
        }
        return true;
    }
    
    if(strcmp(action,"estadoTren") == 0){
        estadoTren(estacion, strtok(NULL, " "));
        return true;
    }
    
    if(strcmp(action, "limpiar") == 0){
        system("clear");
        return true;
    }

    if(strcmp (action, "registroDeTrenes") == 0){
        showTrenReport (estacion);
        return true;
    }
    
    if(strcmp(action, "solicitarAnden") == 0){
        ST_TREN *tren = deleteTrenInQueue( &estacion->colaDeEspera, strtok(NULL, " "));
        solicitaAnden(tren, estacion);
        return true;
    }
   
    return false;
}

void *commandEstacion(void *station){
    ST_ESTACION *estacion = station;
    char *command = requestCommand(estacion);
    
    //pthread_mutex_lock(&mutex);
    
    while(strcmp(command, "actualizar" ) !=0 ){
        while(!processCommand(estacion, command, estacion->new_socket)){
            printf("comando desconocido\n");
            command = requestCommand(estacion);
        }
        command = requestCommand(estacion);    
    }
    
    //pthread_mutex_unlock(&mutex);
    
    return NULL;
}