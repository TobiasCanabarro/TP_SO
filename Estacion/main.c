/* 
 * File:   main.c
 * Author: tobias-pc
 *
 * Created on 7 de octubre de 2019, 18:55
 */

#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>
#include <unistd.h>  
#include <arpa/inet.h>   
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <pthread.h>
#include <unistd.h>

#define MAXBUFFER 1024

#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"
#include "FN_COMANDOS.h"

int main(int argc, char** argv) {
  
    servidor(argv[1]);
    
    return (EXIT_SUCCESS);
}

