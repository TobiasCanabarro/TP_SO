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

void showQueue(ST_NODO *lista);

void subtractFuel(ST_ESTACION *estacion);

void insertOrdered(ST_NODO **lista, ST_TREN *tren);

ST_TREN *search(ST_NODO *lista, char *nomTren);

ST_TREN *delete (ST_NODO **lista, char *nomTren);

#endif /* FN_COLADEESPERA_H */
