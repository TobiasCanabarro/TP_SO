#ifndef ESTACIONES_H_INCLUDED
#define ESTACIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "trenes.h"

typedef enum{
    paso,         //Para los trenes que sólo deben pasar por la estación
    ingreso       //Para los trenes que deben cargar/descargar pasajeros
}EM_MOTIVO;

//se guardan los dato de los trenes que pasan por cada estacion

typedef struct {
    int idTren;
    char estacionOrigen[20];
    char estacionDestino[20];
    EM_MOTIVO motivo;
}ST_INFOREG;

//Implementacion de estructura dinamica para cola de espera

typedef struct nodo{
    ST_TREN trenEnCola;
    struct nodo *ste;
}ST_ColaEspera;

typedef struct {
    char nombreEstacion[20];
    ST_ColaEspera colaDeEspera;
    ST_TREN ocupaAnden;
    bool ocupado;
}ST_ESTACION;


#endif // ESTACIONES_H_INCLUDED

