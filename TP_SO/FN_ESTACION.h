
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
    
void muestraMsj(char msj[]);

void sendMsg(int new_socket,char *message);

void recvMsg(int new_socket,char *buffer);

void addSocket(int client_socket[],int new_socket);

void converToStruct(ST_TREN *tren,char *buffer,char *accion);

void showQueue (ST_ESTACION *estacion);

void disconnected(int sd,int client_socket[],fd_set readfds,struct sockaddr_in address,int addrlen,char *buffer);

void registrar(ST_TREN *tren,ST_ESTACION *estacion,int *posQueue);

void processAction (ST_TREN *tren,char *accion,ST_ESTACION *estacion,int *posQueue);

int loadConfig (ST_ESTACION *estacion);

void servidor();

#ifdef __cplusplus
}
#endif

#endif /* FN_ESTACION_H */

