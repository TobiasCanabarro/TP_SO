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
#include <sys/wait.h>
#include <pthread.h>

#define PORT 7400
#define MAXBUFFER 1024
#define MAX_CLIENTS 10
#define COMMAND_LEN 80
#define SUBSTRACFUEL 20

#define ipEstacionA "127.0.0.1"
#define ipEstacionB "192.168.0.35"//"192.168.1.19" <- ip Rober
#define ipEstacionC "127.0.0.1"
#define DELETED -1
#define MAX_SLEEP 1
#define WAIT 10

#define PATH "/home/tobias-pc/NetBeansProjects/Tren"

#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"
#include "FN_COMANDOS.h"
#include "FN_COLADEESPERA.h"

//pthread_mutex_t mutex;

void addClientsSocket(int client_socket[], int new_socket, int *posSocket){
    if(client_socket[*posSocket] == 0){
        client_socket[*posSocket] = new_socket;
        sendMsg(new_socket, "El tren llego bien!\n");
        *posSocket = *posSocket + 1;
    } 
}

void initializeClientSocket(int client_socket[]){
    for(int i=0;i<MAX_CLIENTS;i++){
        client_socket[i] = 0;
    }
}

void sendMsg(int new_socket, char *buffer){
    if(send(new_socket, buffer, strlen(buffer), 0) != strlen(buffer)){
        perror("ERROR EN SEND\n");
    }
}

void recvMsg(int new_socket, char *buffer){
    memset(buffer, '\0', MAXBUFFER + 1);
     int value = recv(new_socket ,buffer, MAXBUFFER + 1, 0);
        if(value == -1){
            perror("ERROR EN RECV\n");
        }
}

void converToStruct(ST_TREN *tren, char *buffer, char *accion){
    memset(tren->infoTren.estacionOrigen, '\0', 30);
    memset(tren->infoTren.estacionDestino, '\0', 30);
    memset(tren->modelo, '\0', 30);
 
    sscanf(buffer, "%s %s %d %d %d %s %s", accion, tren->modelo, &tren->infoTren.idTren, &tren->combustible,
    &tren->tiempoEspera, tren->infoTren.estacionOrigen, tren->infoTren.estacionDestino);
}

void showAnden(ST_ESTACION *estacion){
   printf("--------------------------------------------------------------------\n");;
    printf("\tEstado del Anden :\n");
    if(estacion->ocupado == false){
        printf("ESTA VACIO!\n");
    }
    else{
        showTren( (&estacion->ocupaAnden) );
    }
}

void registrar(ST_TREN *tren, ST_ESTACION *estacion){ 
    strcpy(tren->estado , "espera");
    strcpy(tren->infoTren.motivo, "registrar ");
    insertOrdered((&estacion->colaDeEspera), tren);
}

void completeStateTrainAnden (ST_TREN *tren, ST_ESTACION *estacion){
    printf("SE PUDO SOLICITAR EL ANDEN\n");
    strcpy(tren->estado, "anden");
    strcpy(tren->infoTren.motivo, "solicitarAnden ");
    estacion->ocupaAnden = *tren;
    estacion->ocupado = true;
}

void solicitaAnden(ST_TREN *tren, ST_ESTACION *estacion){
     if(estacion->ocupado == true){
        if((tren->combustible + tren->tiempoEspera) < (estacion->ocupaAnden.combustible + estacion->ocupaAnden.tiempoEspera)){
            completeStateTrainAnden (tren, estacion);
        }else{
            printf("NO SE PUDO SOLICITAR EL ANDEN PORQUE EL TREN QUE ESTA TIENE MAS PRIORIDAD\n");
            strcpy(tren->estado, "espera");
            strcpy(tren->infoTren.motivo, "registrar ");
            insertOrdered(&estacion->colaDeEspera, tren); 
        }
    }
    else{
        completeStateTrainAnden (tren, estacion);
    }
}

void initializaEstacion (ST_ESTACION *estacion){
    estacion->ocupado = false;
    estacion->ocupaAnden.infoTren.idTren = 0;
    (estacion->colaDeEspera) = NULL;
}

int loadConfigEstacion (ST_ESTACION *estacion, char *path){
    strcpy(estacion->nombreEstacion , path);
    printf("-----------------------------------%s------------------------------------\n", estacion->nombreEstacion);
    int port = getPort(estacion->nombreEstacion);
    return port;
}

void createThread(ST_ESTACION *estacion){  
    
    pthread_t consolaEstacion;
    //pthread_mutex_init(&mutex, NULL);
    pthread_create(&consolaEstacion, NULL, commandEstacion, estacion);
    sleep(MAX_SLEEP);
    pthread_join(consolaEstacion, NULL);
    
    pthread_cancel(consolaEstacion);
    printf("--------------------------------------------------------------------\n");
    
}

void killProcess(int pID){
    char *command = (char*)malloc(sizeof(char)*COMMAND_LEN + 1);
    char *aux = (char*)malloc(sizeof(char)*COMMAND_LEN + 1);
    strcpy(command, "kill -TERM ");
    sprintf(aux, "%d", pID );//system("kill -TERM <pid>");
    command = strcat(command, aux);
    system(command);
}

void createProcess(ST_TREN *tren){
    int pID = fork(); 
    int timeWait = WAIT;
    switch(pID){
        case -1 :
            printf("ERROR AL CREAR EL PROCESO TREN\n");
            break;
        case 0://HIJO    
            execlp(PATH, tren->infoTren.motivo, tren->modelo, NULL);
            tren->pID = getpid();
            sleep(MAX_SLEEP);
            break;
        case 1://PADRE
            wait(&timeWait);
            break;
    }
}

char *createPath (ST_ESTACION *estacion){
    char *path = (char*)malloc(sizeof(char) * 100);
    strcpy(path, "registroDeTrenes");
    path = strcat(path, estacion->nombreEstacion);
    path = strcat(path, ".txt");
    return path;
}

void trenReport(ST_ESTACION *estacion, char *buffer){
    char *path = createPath(estacion);
    FILE *pFile = modeOpenFile(path, "w");
    buffer = strcat(buffer, "\n");
    fputs(buffer, pFile);
    fclose(pFile);
}

char *converToChar(ST_TREN *tren, char *accion){
    char *linea = (char*)malloc(sizeof(char) * MAXBUFFER + 1);
    sprintf(linea, "%s %s %d %d %d %s %s", accion, tren->modelo, tren->infoTren.idTren, tren->combustible, 
    tren->tiempoEspera, tren->infoTren.estacionOrigen, tren->infoTren.estacionDestino);
    
    return linea;
}

void processAction (ST_TREN *tren, char *accion, ST_ESTACION *estacion){
    
    if(strcmp(accion, "registrar") == 0){
        registrar(tren, estacion);
    }
    if(strcmp(accion, "solicitarAnden") == 0){
        solicitaAnden(tren, estacion);
    }
    subtractFuel(estacion);
}

void processTren(ST_ESTACION *estacion, ST_TREN *tren, char *buffer, char *accion){
    sendMsg(tren->new_socket, msgCat("El tren ", tren->modelo, " reside en la " , estacion->nombreEstacion));
    //createProcess(tren); Temporal
    processAction(tren, accion, estacion);   
}

void processBuffer(ST_TREN *tren, ST_ESTACION *estacion, char *buffer, int new_socket){
    
    char *accion = (char*)malloc(sizeof(char) * 15);
    tren->new_socket = new_socket;
    estacion->new_socket = new_socket;
    
    trenReport(estacion, buffer);
    
    converToStruct(tren, buffer, accion);
    
    printf("El %s llego de la %s\n", tren->modelo, tren->infoTren.estacionOrigen);
    
    processTren(estacion, tren, buffer, accion);  
}

void servidor(char *argv){
    ST_TREN *tren = (ST_TREN*)malloc(sizeof(ST_TREN));
    ST_ESTACION estacion;

    initializaEstacion(&estacion);
    int port = loadConfigEstacion(&estacion, argv);
   
    int posSocket = 0;
    int opt = 1;
    int master_socket , addrlen , new_socket , client_socket[MAX_CLIENTS] , activity, sd, max_sd;
 
    struct sockaddr_in address;
    
    char buffer[MAXBUFFER + 1];
    memset(buffer, '\0', MAXBUFFER + 1);

    fd_set readfds;

    initializeClientSocket(client_socket);
    
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){
        perror("ERROR AL PEDIR EL SOCKET\n");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){
        perror("ERROR EN EL SETSOCKPOT\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );
    
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("ERROR EN EL BIND\n");
        exit(EXIT_FAILURE);
    }
    
    printf("ESCUCHANDO EN  : %s(%d)\n", estacion.nombreEstacion, port);

    if (listen(master_socket, 3) < 0){
        perror("ERROR EN EL LISTEN\n");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    printf("ESPERANDO TRENES ...\n");
    
    while(1){
        FD_ZERO(&readfds);//limpia el socket
        FD_SET(master_socket, &readfds);//add master socket to set
        
        max_sd = master_socket;

        for (int i = 0 ; i < MAX_CLIENTS ; i++){//add child sockets to set
            sd = client_socket[i];//socket descriptor
            
            if(sd > 0){
                FD_SET( sd , &readfds);
            }
            
            if(sd > max_sd){//highest file descriptor number, need it for the select function
                max_sd = sd;
            }
        }

        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if((activity < 0) && (errno != EINTR)){
            printf("ERROR EN EL SELECT\n");
        }

        if(FD_ISSET(master_socket, &readfds)){
            if((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){
                perror("ERROR EN EL ACCEPT\n");
                exit(EXIT_FAILURE);
            }
            
            recvMsg(new_socket, buffer);

            addClientsSocket (client_socket, new_socket, &posSocket);

            processBuffer(tren, &estacion, buffer, new_socket);

            //createThread (&estacion);
            commandEstacion ( &estacion );
            
        }
    }
}