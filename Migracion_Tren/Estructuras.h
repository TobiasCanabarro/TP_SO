#ifndef ESTRUCTURAS_H_INCLUDED
#define ESTRUCTURAS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio_ext.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

//#define IPaConectar "192.168.56.1"
#define LONG_COMANDO 1024
#define LONGTREN 1024
//#define PUERTO 7800

typedef enum{
    paso,         //Para los trenes que sólo deben pasar por la estación
    ingreso       //Para los trenes que deben cargar/descargar pasajeros
}EM_MOTIVO;

 typedef enum {
    espera,
    transito,
    anden,
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

#endif // ESTRUCTURAS_H_INCLUDED
