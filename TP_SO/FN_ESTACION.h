
/* 
 * File:   FN_ESTACION.h
 * Author: tobias-pc
 *
 * Created on 7 de octubre de 2019, 19:49
 */

#ifndef FN_ESTACION_H
#define FN_ESTACION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros


#define PORT 7400
#define MAXBUFFER 1024

#include "ESTRUCTURA.h"
#include "FN_TRENES.h"    
    
void inicializaClientSockets(int client_socket[]);
    
void sendMsg(int new_socket,char *message);

void recvMsg(int new_socket,char *buffer);

void addSocket(int client_socket[],int new_socket);

void converToStruct(ST_TREN *tren,char *buffer,char *accion);

void disconnected(int sd,int client_socket[],fd_set readfds,struct sockaddr_in address,int addrlen,char *buffer);

void registrar(ST_TREN *tren,ST_ESTACION *estacion);

void processAction (ST_TREN *tren,char *accion,ST_ESTACION *estacion);

int loadConfig (ST_ESTACION *estacion,char *argv);

void createThread(ST_ESTACION *estacion);

void killProcess(int pID);

void createProcess(ST_TREN *tren);

void processAction (ST_TREN *tren, char *accion, ST_ESTACION *estacion);

void addClientsSocket(int client_socket[], int new_socket, int *posSocket);

void servidor(char *argv, ST_ESTACION *estacion);

void processTren(ST_ESTACION *estacion, ST_TREN *tren, char *buffer, char *accion);

void processBuffer(ST_TREN *tren, ST_ESTACION *estacion, char *buffer, int new_socket);

char *converToChar (ST_TREN *tren, char *accion);

void solicitaAnden (ST_TREN *tren, ST_ESTACION *estacion);

#ifdef __cplusplus
}
#endif

#endif /* FN_ESTACION_H */

