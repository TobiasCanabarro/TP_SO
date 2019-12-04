/* 
 * File:   ESTRUCTURA.h
 * Author: tobias-pc
 *
 * Created on 7 de octubre de 2019, 19:50
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "FN_ESTACION.h"
#include "FN_TRENES.h"
#include <stdbool.h>

#define MAXTRENES 5

#ifndef ESTRUCTURA_H
#define ESTRUCTURA_H

#ifdef __cplusplus
extern "C" {
#endif
    
//se guardan los dato de los trenes que pasan por cada estacion

typedef struct {
    int idTren;
    char estacionOrigen[30];
    char estacionDestino[30];
    char motivo[30]; // paso, ingreso. Se va a usar para el registro de actividad de la estacion
}ST_INFOREG;

typedef  struct {
    ST_INFOREG infoTren;
    char modelo[30];
    int combustible;
    int tiempoEspera;
    int pID;
    int new_socket;
    char estado[10]; // espera (COLA DE ESPERA) , transito, anden 
    char *bufferToSend;
}ST_TREN;
   
typedef struct nodo{
    ST_TREN *tren;
    int tiempoEnCola;
    struct nodo *ste;
}ST_NODO;

typedef struct {
    char nombreEstacion[20];
    ST_NODO *colaDeEspera;
    ST_TREN ocupaAnden;
    bool ocupado;
    int new_socket;
}ST_ESTACION;

#ifdef __cplusplus
}
#endif

#endif /* ESTRUCTURA_H */

