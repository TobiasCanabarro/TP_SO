#ifndef FN_COMANDOS_H
#define FN_COMANDOS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>  
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>
#include <unistd.h>   
#include <arpa/inet.h>  
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"
#include "FN_COLADEESPERA.h"

void *commandEstacion(void *estacion);

char *msgCat(const char *cad1, const char *cad2, const char*cad3, const char *cad4);




#ifdef __cplusplus
}
#endif

#endif /* FN_COMANDOS_H */

