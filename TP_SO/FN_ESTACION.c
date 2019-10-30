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

#define PORT 7400
#define MAXBUFFER 1024
#define MAX_CLIENTS 5

#define ipEstacionA "127.0.0.1"
#define ipEstacionB "192.168.1.16"
#define ipEstacionC "127.0.0.1"

#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"


void initializeClientSocket(int client_socket[]){
    for(int i=0;i<MAX_CLIENTS;i++){
        client_socket[i] = 0;
    }
}

void muestraMsj(char msj[]){
    printf("%s\n",msj);
}

void sendMsg(int new_socket,char *message){
    if(send(new_socket, message, strlen(message), 0) != strlen(message)){
        perror("send");
    }
}

void recvMsg(int new_socket,char *buffer){
    memset(buffer,'\0',MAXBUFFER+1);
     int value = recv(new_socket,buffer,MAXBUFFER+1,0);
        if(value == -1){
            perror("Error en RECV\n");
        }
}

void addSocket(int client_socket[],int new_socket){
    for (int i = 0; i < MAX_CLIENTS; i++){
            if( client_socket[i] == 0 ){
                client_socket[i] = new_socket;
                printf("Adding to list of sockets as %d\n" , i);
                break;
                }
            }   
}

void converToStruct(ST_TREN *tren,char *buffer,char *accion){
    /*  accion modelo idTren timepoEspera Combustible estacionDestino estacionOrigen */ 
    sscanf(buffer,"%s%s%d%d%d%s%s",accion,tren->modelo,&tren->infoTren.idTren,&tren->tiempoEspera,
    &tren->combustible,tren->infoTren.estacionOrigen,tren->infoTren.estacionDestino);
}

void showQueue (ST_ESTACION *estacion){
    printf("--------------------------------------------------------------------------\n");
    printf("\tEstado de la Cola de Espera para el Anden :\n");
    for(int i=0;i<MAXTRENES;i++){
        printf("%s %d %d %d %s %s \n",estacion->colaDeEspera[i].modelo,estacion->colaDeEspera[i].infoTren.idTren,estacion->colaDeEspera[i].tiempoEspera,estacion->colaDeEspera[i].combustible,estacion->colaDeEspera[i].infoTren.estacionDestino,estacion->colaDeEspera[i].infoTren.estacionOrigen);
    }
}

void showAnden(ST_ESTACION *estacion){
    printf("--------------------------------------------------------------------------\n");
    printf("\tEstado del Anden :\n");
    if(estacion->ocupaAnden.infoTren.idTren == 0){
        printf("Esta VACIO\n");
    }
    else{
        printf("Modelo : %s\n ID del Tren : %d\n Tiempo de Espera : %d m\n Combustible : %d L\n Estacion Origen : %s\n Estacion Destino : %s\n",
                estacion->ocupaAnden.modelo,estacion->ocupaAnden.infoTren.idTren,estacion->ocupaAnden.tiempoEspera,estacion->ocupaAnden.combustible,estacion->ocupaAnden.infoTren.estacionOrigen,estacion->ocupaAnden.infoTren.estacionDestino);
    }
}

void disconnected(int sd,int client_socket[],fd_set readfds,struct sockaddr_in address,int addrlen,char *buffer){
    int valread;
   for (int i = 0; i < MAX_CLIENTS; i++){
            sd = client_socket[i];
            if (FD_ISSET( sd , &readfds)){
                if ((valread = read( sd , buffer, MAXBUFFER)) == 0){
                    getpeername(sd , (struct sockaddr*)&address ,(socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" ,inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
                    close(sd);
                    client_socket[i] = 0;
                }
            }
   }
}

void registrar(ST_TREN *tren,ST_ESTACION *estacion,int *posQueue){
    estacion->colaDeEspera[*posQueue] = *tren;
    *posQueue++;
}

void solicitaAnden(ST_TREN *tren,ST_ESTACION *estacion,int *posQueue){
    if(estacion->ocupaAnden.infoTren.idTren == 0){
        estacion->ocupaAnden = *tren;
    }
    else{
        estacion->colaDeEspera[*posQueue] = *tren;
        *posQueue++;
    }
}

void processAction (ST_TREN *tren,char *accion,ST_ESTACION *estacion,int *posQueue){
    if(strcmp(accion,"registrar")==0){
        registrar(tren,estacion,posQueue);
    }
    if(strcmp(accion,"solicitar")==0){
        solicitaAnden(tren,estacion,posQueue);
    }
    if(strcmp(accion,"estado")==0){
        
    }
    if(strcmp(accion,"partir")==0){
        
    }

}

void initializaQueue (ST_ESTACION *estacion){
    estacion->ocupaAnden.infoTren.idTren = 0;
    for(int i=0;i<MAX_CLIENTS;i++){
        estacion->colaDeEspera[i].infoTren.idTren = 0;
    }
}

char *deleteContraBarra (char *nom){
    char *aux = nom;
    char *token = strtok(aux,"\n");
    return token;
}

int loadConfig (ST_ESTACION *estacion){
    printf("--------------------------------------------------\n");
    char *nomEstacion = (char*)malloc(sizeof(char)*15);
    memset(estacion->nombreEstacion,'\0',20);
    printf("\tConfiguracion de ");
    scanf("%s",nomEstacion);
    
    strcpy(estacion->nombreEstacion , readFileConfig(nomEstacion));
    
    char *aux = deleteContraBarra(estacion->nombreEstacion);//provisorio :D
    strcpy(estacion->nombreEstacion,aux);
    int port = getPort(estacion->nombreEstacion);
    return port;
}

char *converTochar(ST_TREN *tren,char *accion){
    char *linea = (char*)malloc(sizeof(char)*MAXBUFFER);
    sprintf(linea,"%s %s %d %d %d %s %s",accion,tren->modelo,tren->infoTren.idTren,tren->combustible,tren->tiempoEspera,tren->infoTren.estacionOrigen,tren->infoTren.estacionDestino);
    return linea;
}

void cleanStruct(ST_TREN *tren){
    tren->combustible = 0;
    memset(tren->modelo,'\0',20);
    tren->tiempoEspera = 0;
    memset(tren->infoTren.estacionDestino ,'\0',20);
    memset(tren->infoTren.estacionOrigen ,'\0',20);
    tren->infoTren.idTren = 0;
}

void processTren(ST_TREN *tren,ST_ESTACION *estacion,int *posQueue,char *buffer){
    char *accion = (char*)malloc(sizeof(char)*15);
    printf("Buffer : %s\n",buffer);
    
    cleanStruct(tren);
    converToStruct(tren,buffer,accion);
    //printf("Estacion Destino : %s\n",tren->infoTren.estacionDestino);
    //printf("Nombre de la Estacion : %s\n",estacion->nombreEstacion);
    
    if(strcmp(estacion->nombreEstacion,tren->infoTren.estacionDestino)!=0){
        int port = getPort(tren->infoTren.estacionDestino);
        char *ip = getIP(tren->infoTren.estacionDestino);
        char *value = converTochar(tren,accion);
        printf("Value : %s\n",value);
        printf("Puerto : %d\n",port);
        cliente(value,port,ip);
    }
    else{
        converToStruct(tren,buffer,accion);
        processAction(tren,accion,estacion,posQueue);
        if(strcmp(accion,"registrar")==0){
            showQueue(estacion);
        }
        if(strcmp(accion,"solicitar")==0){
             showAnden(estacion);
        }
    }
}

void servidor(){
    ST_ESTACION *estacion =(ST_ESTACION*)malloc(sizeof(ST_ESTACION));
    ST_TREN *tren = (ST_TREN*)malloc(sizeof(ST_TREN));
    initializaQueue(estacion);
    int port = loadConfig(estacion);
    
    int posQueue = 0;
    int opt = 1;
    int posSocket = 0;
    int master_socket , addrlen , new_socket , client_socket[30] , activity, valread , sd,max_sd;
    
    struct sockaddr_in address;
    
    char buffer[MAXBUFFER+1];
    memset(buffer,'\0',MAXBUFFER+1);

    fd_set readfds;

    initializeClientSocket(client_socket);
    
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0){
        perror("ERROR en la creacion del socket\n");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 ){
        perror("ERROR en el setsockopt\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    //address.sin_port = htons( PORT );
    address.sin_port = htons(port);
    
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){
        perror("ERROR en el bind\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Escuchando en  : %s(%d)\n",estacion->nombreEstacion,port);

    if (listen(master_socket, 3) < 0){
        perror("EROR en el listen\n");
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(address);
    printf("Esperando conecciones ...\n");
    
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

        if ((activity < 0) && (errno!=EINTR)){
            printf("ERROR en el select\n");
        }

        if (FD_ISSET(master_socket, &readfds)){
            if ((new_socket = accept(master_socket,(struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
                perror("ERROR en el accept\n");
                exit(EXIT_FAILURE);
            }
            
            recvMsg(new_socket,buffer);
            
            //converToStruct(tren,buffer,accion);
            
            //processAction(tren,accion,estacion,&posQueue);
            processTren(tren,estacion,&posQueue,buffer);
            
            sendMsg(new_socket,"El tren llego bien!\n");
            
            if(client_socket[posSocket] == 0){
                client_socket[posSocket] = new_socket;
                printf("Tren añadido al socket : %d\n",posSocket);
                posSocket ++;
            }
            
            //addSocket(client_socket,new_socket);
            
            }
        }
            disconnected(sd,client_socket,readfds,address,addrlen,buffer);
        }