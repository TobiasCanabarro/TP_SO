/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ESTRUCTURAS.h
 * Author: tobias-pc
 *
 * Created on 27 de noviembre de 2019, 11:50
 */

#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#ifdef __cplusplus
extern "C" {
#endif

//se guardan los dato de los trenes que pasan por cada estacion

typedef struct {
    int idTren;
    char estacionOrigen[20];
    char estacionDestino[20];
    char motivo[10]; // paso, ingreso. Se va a usar para el registro de actividad de la estacion
}ST_INFOREG;

typedef  struct {
    ST_INFOREG infoTren;
    char modelo[20];
    int combustible;
    int tiempoEspera;
    int pID;
    int new_socket;
    char estado[10]; // espera (COLA DE ESPERA) , transito, anden 
}ST_TREN;


#ifdef __cplusplus
}
#endif

#endif /* ESTRUCTURAS_H */

