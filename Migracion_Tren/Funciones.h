#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

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

void discardChars();

FILE *modeOpenFile(const char *path,const char *mode);

char *leeFileConfig(char *path);

char *creaTren();

char * pideComando();

char * sacoPath(char *linea);

char *acciones (char* accion,char *comando);

char *sacoAccion(char *cad,char separador);

char *leeFileConfig(char *path);

char *comando();

int cliente(char *tren,char *puerto);







#endif // FUNCIONES_H_INCLUDED
