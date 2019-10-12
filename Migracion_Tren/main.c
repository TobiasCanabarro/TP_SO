#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio_ext.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Estructuras.h"
#include "Funciones.h"

#define IPaConectar "127.0.0.1"
#define LONG_COMANDO 1024
#define LONGTREN 1024
#define PUERTO 7400



int main(){

    char *tren = comando();

    if(strcmp(tren,"desconocido")==0){
        printf("Comando desconocido\n");
        return 1;
    }
    if(strcmp(tren,"exit")==0){
        return 0;
    }

    while(strcmp(tren,"exit")!= 0){

        cliente(tren);

        strcpy(tren , comando());

        if(strcmp(tren,"desconocido")==0){
            printf("Comando desconocido\n");
            return 1;
        }
        if(strcmp(tren,"exit")==0){
            return 0;
        }
    }

    return 0;
}

