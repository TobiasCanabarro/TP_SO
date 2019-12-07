
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

/**
*@brief Abre el archivo de configuracion en el modo requerido
*@param A Path del archivo
*@param B modo de apertura del archivo
*@return Puntero del archivo de tipo *FILE
*/

FILE *modeOpenFile(char *path,char *mode);



 char *myStrtok(char *linea, char separador);

 char * quitPath(char *linea);

 char *readFileConfig(char *path);

/**
 * @brief Asigna un puerto a determinada estacion.
 * @param A Nombre de estacion.
 * @return Un entero del numero del puerto.
 */


 int getPort(char *puerto);

 /**
 * @brief Asigna una ip a determinada estacion.
 * @param A Nombre de estacion.
 * @return Una cadena con la ip.
 */

 char *getIP (char *nomEstacion);

 /**
 * @brief Crea una nueva conexion de tipo cliente.
 * @param Una cadena con los datos del tren
 * @param Un entero con el puerto.
 * @param Una cadena con la ip.
 * @return Devuelve un entero como identificador si fue exitasa la operacion 0/1.
 */

 int cliente(char *tren, int puerto, char *ip);


#ifdef __cplusplus
}
#endif

#endif /* FN_TRENES_H */

