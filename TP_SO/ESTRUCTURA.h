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

#define MAXTRENES 2

#ifndef ESTRUCTURA_H
#define ESTRUCTURA_H

#ifdef __cplusplus
extern "C" {
#endif
    
typedef enum{
    paso,         //Para los trenes que sólo deben pasar por la estación
    ingreso       //Para los trenes que deben cargar/descargar pasajeros
}EM_MOTIVO;
    
 typedef enum {
    espera,        //0
    transito,      //1
    anden,         //2
}EM_ESTADO;

//se guardan los dato de los trenes que pasan por cada estacion

typedef struct {
    int idTren;
    char estacionOrigen[20];
    char estacionDestino[20];
    EM_MOTIVO motivo;
}ST_INFOREG;

typedef  struct {
    ST_INFOREG infoTren;
    char modelo[20];
    int combustible;
    int tiempoEspera;
    EM_ESTADO estado;
}ST_TREN;
   
/*Implementacion de estructura dinamica para cola de espera
typedef struct nodo{
    ST_TREN trenEnCola;
    struct nodo *ste;
}ST_ColaEspera;
*/
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

