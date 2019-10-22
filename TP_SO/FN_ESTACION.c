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
    &tren->combustible,tren->infoTren.estacionDestino,tren->infoTren.estacionOrigen);
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
        printf("Modelo :%s\n ID del Tren : %d\n Tiempo de Espera : %d\n Combustible :%d\n Estacion Destino :%s\n Estacion Origen :%s\n",
                estacion->ocupaAnden.modelo,estacion->ocupaAnden.infoTren.idTren,estacion->ocupaAnden.tiempoEspera,estacion->ocupaAnden.combustible,estacion->ocupaAnden.infoTren.estacionDestino,estacion->ocupaAnden.infoTren.estacionOrigen);
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

int MystrCmp (char *cad1,char *cad2){

    char *auxCad1 = cad1;
    char *auxCad2 = cad2;

    int longCad1 = strlen(cad1);
    int longCad2 = strlen(cad2);

    int cont = 0;

   while(*auxCad1 && *auxCad2){
        if(*auxCad1 > *auxCad2){
            return 1;//contMay++
        }
        if(*auxCad1 < *auxCad2){
            return -1;
        }
        cont++;
        auxCad1++;
        auxCad2++;
    }

    if(longCad1 == cont && longCad2 == cont){
        return 0;
    }

    if(longCad1 == cont){
        return 1;
    }
    if(longCad2 == cont ){
        return -1;
    }

    return 0;
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

void servidor(){
    ST_ESTACION *estacion =(ST_ESTACION*)malloc(sizeof(ST_ESTACION));
    ST_TREN *tren = (ST_TREN*)malloc(sizeof(ST_TREN));
    initializaQueue(estacion);
    int port = loadConfig(estacion);
    
    int posQueue = 0;
    int opt = 1;
    int posSocket = 0;
    int master_socket , addrlen , new_socket , client_socket[30] , activity, valread , sd,max_sd;
    char *accion = (char*)malloc(sizeof(char)*15);
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
            
            converToStruct(tren,buffer,accion);
            
            processAction(tren,accion,estacion,&posQueue);
            
            showQueue(estacion);
            
            showAnden(estacion);
            
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