/* 
 * File:   FN_COLADEESPERA.h
 * Author: tobias-pc
 *
 * Created on 25 de noviembre de 2019, 18:33
 */

#ifndef FN_COLADEESPERA_H
#define FN_COLADEESPERA_H

#include <stdio.h>
#include <string.h>  
#include <stdlib.h>
#include <stdbool.h>
#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"
#include "FN_COMANDOS.h"

/**
 * @brief Muestra la cola de espera.
 * @lista Estructura de cola.
 * @return void
 */
void showQueue(ST_NODO *lista);

/**
 * @brief Disminuye el combustible de todos los tren que estan en la estacion.
 * @estacion La estructura de la estacion.
 * @return void
 */
void subtractFuel(ST_ESTACION *estacion);

/**
 * @brief Inserte un tren en lista de forma ordenada.
 * @lista La cola de espera.
 * @tren Un puntero de la estructura del tren.
 * @return void
 */
void insertOrdered(ST_NODO **lista, ST_TREN *tren);


/**
 * @brief Busca un tren en la cola de espera y lo retorna.
 * @lista La cola de espera.
 * @nomTren nombre del tren.
 * @return tren buscado
 */
ST_TREN *searchInQueue(ST_NODO *lista, char *nomTren);

/**
 * @brief Elimina un tren deseado de la cola de espera.
 * @lista La cola de espera.
 * @nomTren Nombre del tren.
 * @return El tren que fue eliminado
 */
ST_TREN *deleteTrenInQueue(ST_NODO **lista, char *nomTren);

#endif /* FN_COLADEESPERA_H */
