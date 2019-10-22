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
    char estacionOrigen[20];
    char estacionDestino[20];
    char motivo[10];//paso, ingreso
}ST_INFOREG;

typedef  struct {
    ST_INFOREG infoTren;
    char modelo[20];
    int combustible;
    int tiempoEspera;
    char estado[10];//espera, transito,anden
}ST_TREN;
   
typedef struct {
    char nombreEstacion[20];
    ST_TREN colaDeEspera[MAXTRENES];
    ST_TREN ocupaAnden;
    bool ocupado;
}ST_ESTACION;

#ifdef __cplusplus
}
#endif

#endif /* ESTRUCTURA_H */

