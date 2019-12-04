/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FUNCIONES.h
 * Author: tobias-pc
 *
 * Created on 27 de noviembre de 2019, 11:43
 */

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio_ext.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ESTRUCTURAS.h"

#define ipEstacionA "127.0.0.1"
#define ipEstacionB "127.0.0.1"//"192.168.1.16"
#define ipEstacionC "127.0.0.1"

#define LONG_COMANDO 1024
#define LONGTREN 1024
#define PUERTO 7400

char *creaTren(char *argv[]);

char *obtieneIP(char *nomEstacion);

char * obtienePuerto(char *nomEstacion);

ST_TREN *convToStruct(char *tren, const char *formato);

void trenCreado(char *tren);

int cliente(char *tren,char *puerto, char *ip);

void onLine(bool flag);

#endif /* FUNCIONES_H */
