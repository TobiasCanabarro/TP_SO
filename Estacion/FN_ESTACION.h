
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

/**
 * @brief Inicialiaza el vector de los clientes sockets.
 * @client_socket Vector de clientes sockets.
 * @return 0
 */    
void inicializaClientSockets(int client_socket[]);

/**
 * @brief Inicializa los valores basicos a la estacion.
 * @estacion Estructura de la estacion.
 * @return void.
 */
void initializaEstacion (ST_ESTACION *estacion);

/**
 * @brief Envia un buffer un servidor/cliente Sockets.
 * @new_socket Socket de la conexion.
 * @message Una cadena con el buffer.
 * @return void.
 */
void sendMsg(int new_socket,char *message);

/**
 * @brief Recibe el buffer de un servidor/cliente Sockets.
 * @new_socket Socket de la conexion.
 * @buffer Una cadena con el buffer.
 * @return void
 */
void recvMsg(int new_socket,char *buffer);

/**
 * @brief Añade un socket al vector de clientes.
 * @client_socket Vector con los clientes
 * @new_socket Nuevo socket.
 * @return void.
 */
void addSocket(int client_socket[],int new_socket);

/**
 * @brief Convierte una cadena de texto a una estructura del tipo tren.
 * @tren Estructura del tipo tren.
 * @buffer Datos del tren en forma de cadena.
 * @accion Accion solicitada
 * @return void
 */
void converToStruct(ST_TREN *tren,char *buffer,char *accion);

/**
 * @brief Muestra el estado del anden.
 * @estacion Estructura de la estacion.
 * @return void
 */
void showAnden(ST_ESTACION *estacion);

/**
 * @brief Registra un tren en la estacion, lo ingresa a la cola de espera.
 * @tren Estructura del tren.
 * @estacion Estrutura de la estacion.
 * @return void
 */
void registrar(ST_TREN *tren,ST_ESTACION *estacion);

/**
 * @brief Procesa la accion solicitada y la ejecuta.
 * @tren Estructura del tren.
 * @accion Accion solicitada para ser ejecutada.
 * @estacion Estructura de la estacion
 * @return void
 */
void processAction (ST_TREN *tren,char *accion,ST_ESTACION *estacion);

/**
 * @brief Completa el estado del tren en anden.
 * @tren Estructura del tren.
 * @estacion Estructura de la estacion.
 * @return void
 */
void completeStateTrainAnden (ST_TREN *tren, ST_ESTACION *estacion);

/**
 * @brief Carga la configuracion de la estacion.
 * @argv Cadena de texto del argumento.
 * @estacion Estructura de la estacion.
 * @return void
 */
int loadConfigEstacion (ST_ESTACION *estacion, char *argv);

/**
 * @brief Crea un hilo.
 * @estacion Estructura de la estacion.
 * @return void
 */
void createThread(ST_ESTACION *estacion);

/**
 * @brief Termina el proceso tren.
 * @pID PID del proceso.
 * @return void
 */
void killProcess(int pID);

/**
 * @brief Crea el proceso tren con los datos del tren.
 * @tren Estructura del tren.
* @return void
 */
void createProcess(ST_TREN *tren);

/**
 * @brief Crea el path del archivo de registro de trenes de la estacion.
 * @estacion Estructura de la estacion.
 * @return void
 */
char *createPath (ST_ESTACION *estacion);

/**
 * @brief Registra los trenes que pasan por la estacion.
 * @estacion Estructura de la estacion.
 * @buffer Los datos del tren en forma de cadena de texto.
 * @return void
 */
void trenReport(ST_ESTACION *estacion, char *buffer);

/**
 * @brief Procesa la accion, puede ser registrar o solicitar anden.
 * @tren Estrutura del tren.
 * @accion Accion solicitada.
 * @estacion Estructura de la estacion.
 * @return void
 */
void processAction (ST_TREN *tren, char *accion, ST_ESTACION *estacion);

/**
 * @brief Crea una estacion.
 * @argv Cadena de texto del argumento.
 * @return void
 */
void servidor(char *argv);

/**
 * @brief Procesa el tren llegado a la estacion.
 * @tren Estrutura del tren.
 * @buffer Buffer del socket.
 * @estacion Estructura de la estacion.
 * @accion Accion a realizar.
 * @return void
 */
void processTren(ST_ESTACION *estacion, ST_TREN *tren, char *buffer, char *accion);

/**
 * @brief Procesa la accion, puede ser registrar o solicitar anden.
 * @tren Estrutura del tren.
 * @accion Accion solicitada.
 * @estacion Estructura de la estacion.
 * @return void
 */
void processBuffer(ST_TREN *tren, ST_ESTACION *estacion, char *buffer, int new_socket);

/**
 * @brief Convierte una estructura a una cadena de texto y la retorna.
 * @tren Estrutura del tren.
 * @accion Accion solicitada.
 * @return La estructura convertida a cadena
 */
char *converToChar (ST_TREN *tren, char *accion);

/**
 * @brief El tren solicita el anden de la estacion.
 * @tren Estrutura del tren.
 * @estacion Estructura de la estacion.
 * @return void
 */
void solicitaAnden (ST_TREN *tren, ST_ESTACION *estacion);

#ifdef __cplusplus
}
#endif

#endif /* FN_ESTACION_H */

