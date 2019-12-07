/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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
 * @param Estructura de cola.
 * @return 0
 */

void showQueue(ST_NODO *lista);

/**
 * @brief Resta combustible de los trenes.
 * @param La estructura de la estacion.
 * @return 0
 */

void subtractFuel(ST_ESTACION *estacion);

void sortList(ST_NODO **lista);

ST_TREN *deleteFirst(ST_NODO **lista);

/**
 * @brief Inserte de forma ordenada la lista.
 * @param La cola de espera.
 * @param Un puntero de la estructura del tren.
 * @return 0
 */

void insertOrdered(ST_NODO **lista, ST_TREN *tren);

/**
 * @brief Busca un nodo de la lista (cola de espera).
 * @param La cola de espera.
 * @param Una cadena del nombre del tren.
 * @return Estructura del tren.
 */

ST_TREN *search(ST_NODO *lista, char *nomTren);

/**
 * @brief Elimina el tren de la cola de espera.
 * @param La cola de espera.
 * @param Una cadena del nombre del tren.
 * @return Estructura del tren.
 */

ST_TREN *delete (ST_NODO **lista, char *nomTren);

void clearQueue(ST_NODO **lista);

#endif /* FN_COLADEESPERA_H */
