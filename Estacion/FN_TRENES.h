
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
*@path  Path del archivo
*@mode Modo de apertura del archivo
*@return Puntero del archivo de tipo
*/
FILE *modeOpenFile(char *path, char *mode);

/**
*@brief Obtiene el puerto que se requiere para la conexion, a traves del nombre de la estacion 
*@nomEstacion Nombre de la estacion
*@return Puntero del archivo de tipo *FILE
*/
int getPort(char *nomEstacion);

/**
*@brief Obtiene la ip que se requiere para la conexion, a traves del nombre de la estacion 
*@nomEstacion Nombre de la estacion
*@return Puntero del archivo de tipo *FILE
*/
char *getIP (char *nomEstacion);
 
/**
*@brief Establece una conexion del tipo cliente, a un ip y un puerto en especifico
*@puerto Numero del puerto 
*@ip Cadena de texto
*@return 0 o 1, si se pudo realizar la conexion
*/
int cliente(char *trenLinea, int puerto, char *ip);

/**
*@brief Muestra el estado de un tren en especifico 
*@tren Estructura del tren
*@return void
*/
void showTren(ST_TREN *tren);
 
#ifdef __cplusplus
}
#endif

#endif /* FN_TRENES_H */

