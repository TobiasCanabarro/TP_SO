#ifndef FN_COMANDOS_H
#define FN_COMANDOS_H

#ifdef __cplusplus
extern "C" {
#endif

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

#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"
#include "FN_COLADEESPERA.h"

/**
 * @brief Permite ingresar comandos en la estacion.
 * @estacion La estructura de la estaciin.
 * @return void.
 */
void *commandEstacion(void *estacion);

/**
 * @brief Concatena un mensaje.
 * @cad1 Cadena de texto.
 * @cad2 Cadena de texto.
 * @cad3 Cadena de texto.
 * @cad4 Cadena de texto.
 * @return Mensaje concatenado.
 */
char *msgCat(const char *cad1, const char *cad2, const char*cad3, const char *cad4);

/**
 * @brief Solicita un comando.
 * @estacion Estructura de la estacion.
 * @return El comando solicitado.
 */
char *requestCommand(ST_ESTACION *estacion);

/**
 * @brief Muestra el estado de la estacion.
 * @estacion Estructura de la estacion.
 * @return void.
 */
void showEstacion(ST_ESTACION *estacion);

/**
 * @brief Busca un tren el la estacion.
 * @nomTren B Nombre del tren que se desea buscar.
 * @estacion Estructura de la estacion
 * @return El tren que se solicito.
 */
ST_TREN *searchTren (ST_ESTACION *estacion, char *nomTren);

/**
 * @brief Procesa el comando solicitado.
 * @estacion Estructura de la estacion.
 * @command Comando solicitado.
 * @new_socket Numero del socket que se usa para la conexion.
 * @return true o false, si el comando se pudo procesar.
 */
bool processCommand(ST_ESTACION *estacion, char *command, int new_socket);

/**
 * @brief Muestra el estado de un tren solicitado.
 * @estacion Estructura de la estacion.
 * @nomTren Nombre del tren del cual se quiere ver el estado.
 * @return void.
 */
void estadoTren (ST_ESTACION *estacion, char *nomTren);

/**
 * @brief Muestra la lista de comandos permitidos.
 * @return void.
 */
void commandList();

/**
 * @brief Elimina un tren especifico.
 * @estacion Estructura de la estacion.
 * @nomTren Nombre del tren.
 * @return 0 o 1, Si se pudo eliminar dicho tren.
 */
int deleteTren(ST_ESTACION *estacion,char *nomTren);

/**
 * @brief Envia un tren especifico a una estacion especifica.
 * @estacion Estructura de la estacion.
 * @command Comando solicitado.
 * @new_socket Numero del socket que se usa para la conexion.
 * @return void.
 */
void partirTren(ST_ESTACION *estacion, char *command, int new_socket );

#ifdef __cplusplus
}
#endif

#endif /* FN_COMANDOS_H */

