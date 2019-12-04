/* 
 * File:   main.c
 * Author: tobias-pc
 *
 * Created on 27 de noviembre de 2019, 11:43
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio_ext.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "FUNCIONES.h"
#include "ESTRUCTURAS.h"

#define ipEstacionA "127.0.0.1"
#define ipEstacionB "127.0.0.1"//"192.168.1.16"
#define ipEstacionC "127.0.0.1"
#define LONG_COMANDO 1024
#define LONGTREN 1024
#define PUERTO 7400


int main(int argc, char** argv) {
     
    char *tren = creaTren(argv);
    
    char *ip = obtieneIP(argv[2]);
    
    char *puerto = obtienePuerto(argv[2]);
    
    printf("IP %s\n",ip);
    printf("PUERTO %s\n",puerto);
    
    trenCreado(tren);
      
    cliente(tren, puerto, ip);
    
    onLine(true);
    
    return (EXIT_SUCCESS);
}

