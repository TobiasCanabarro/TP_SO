
/* 
 * File:   FN_TRENES.h
 * Author: tobias-pc
 *
 * Created on 7 de octubre de 2019, 19:48
 */

#ifndef FN_TRENES_H
#define FN_TRENES_H

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
#include "FN_ESTACION.h"
#include "FN_TRENES.h"
    
FILE *modeOpenFile(char *path, char *mode);
 
 int getPort(char *nomEstacion);
 
 char *getIP (char *nomEstacion);
 
 int cliente(char *trenLinea, int puerto, char *ip);
 
 void showTren(ST_TREN *tren);
 
#ifdef __cplusplus
}
#endif

#endif /* FN_TRENES_H */

