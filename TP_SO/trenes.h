#ifndef TRENES_H_INCLUDED
#define TRENES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "estaciones.h"

typedef enum {
    espera,
    transito,
    anden,
}EM_ESTADO;


typedef  struct {
    ST_INFOREG infoTren;
    char modelo[20];
    float combustible;
    float tiempoEspera;
    EM_ESTADO estado;
}ST_TREN;


#endif // TRENES_H_INCLUDED
