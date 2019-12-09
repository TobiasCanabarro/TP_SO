/* 
 * File:   FUNCIONES.h
 * Author: tobias-pc
 *
 * Created on 27 de noviembre de 2019, 11:43
 */

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio_ext.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ESTRUCTURAS.h"

#define ipEstacionA "127.0.0.1"
#define ipEstacionB "127.0.0.1"//"192.168.1.16"
#define ipEstacionC "127.0.0.1"

#define LONG_COMANDO 1024
#define LONGTREN 1024
#define PUERTO 7400

/**
*@brief Abre el archivo de configuracion en el modo requerido
*@path  Path del archivo
*@mode Modo de apertura del archivo
*@return Puntero del archivo de tipo
*/
FILE *modeOpenFile(const char *path,const char *mode);

/**
*@brief Abre y lee el archivo configuracion, carga los valores necesarios para iniciar la estacion
*@path  Path del archivo que se desea leer
*@return una cadena de texto con los valores de la estacion
*/
char *leeFileConfig(char *path);

/**
*@brief Valida si el comando solicitado se puede realizar
*@path  Accion a realizar
*@return una cadena de texto con los valores de la estacion
*/
bool conoceComando(char *accion);

/**
*@brief Parcea los argumento ingresados al momento de la compilacion
*@tren  Estructura del tren
*@argv  Argumentos de la compilacion
*@return el tren creado
*/
char *parceoArgv(char *tren, char *argv[]);

/**
*@brief Abre y lee el archivo configuracion, carga los valores necesarios para iniciar la estacion
*@argv  Argumentos de la compilacion
*@return NULL, si no se pudo procesar los argumentos y si se pudo retorna el tren
*/
char *procesoArgumentos (char *argv[]);

/**
*@brief Genera un tren, con el argunmento de la compilacion
*@argv  Argumentos de la compilacion
*@return El tren creado
*/
char *creaTren(char *argv[]);

/**
*@brief Genera un ip, con el nombre de la estacion
*@nomEstacion  Nombre de la estacion
*@return El tren creado
*/
char *obtieneIP(char *nomEstacion);

/**
*@brief Genera un puerto, con el nombre de la estacion
*@nomEstacion  Nombre de la estacion
*@return El tren creado
*/
char * obtienePuerto(char *nomEstacion);

/**
*@brief Convierte una cadena a estructura
*@tren  Estructura del tren
*@formato tipos de datos de los campos de la estructura
*@return La estrucutra carga del tren con los datos
*/
ST_TREN *convToStruct(char *tren, const char *formato);

/**
*@brief Muestra el tren creado
*@tren  Estructura del tren
*@return void
*/
void trenCreado(char *tren);

/**
*@brief Establece una conexion del tipo cliente
*@tren  Datos del tren en formato cadena
*@formato tipos de datos de los campos de la estructura
*@return 0, si se pudo establecer la conexion y 1, si no se pudo establecer la conexion
*/
int cliente(char *tren, char *puerto, char *ip);

/**
*@brief Mantiene el proceso en un bucle
*@flag  true o false, si se establece un bucle 
*@return void
*/
void onLine(bool flag);

#endif /* FUNCIONES_H */
