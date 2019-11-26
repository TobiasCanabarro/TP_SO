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

#define ipEstacionA "127.0.0.1"
#define ipEstacionB "127.0.0.1"//"192.168.1.16"
#define ipEstacionC "127.0.0.1"

#define LONG_COMANDO 1024
#define LONGTREN 1024
#define PUERTO 7400

void discardChars(){
    char c;
    while((c = getchar()) != '\n' && c != EOF);
    return;
}

char *creaTren(){
    char *tren = (char*)malloc(sizeof(char)*LONGTREN+1);
    memset(tren,'\0',LONGTREN+1);
    printf("Modelo de tren e ID :");
    __fpurge(stdin);
    int trenLen = strlen(tren);
    getline(&tren,(void*)&trenLen,stdin);
    //gets(tren);
    return tren;
}

char * pideComando(){
    char *comando = (char*)malloc(sizeof(char)*LONG_COMANDO+1);
    memset(comando,'\0',LONG_COMANDO+1);
    printf("creaTren@ : ");
    __fpurge(stdin);
    gets(comando);
    return comando;
}

char * sacoPath(char *linea){
    char *aux = linea;
    while(*aux && *aux != ' '){
        aux++;
    }
    aux++;
    return aux;
}

FILE *modeOpenFile(const char *path,const char *mode){
    FILE *pFile = NULL;
    pFile = fopen(path,mode);
    if(pFile == NULL){
        printf("Error al leer el archivo configuracion del tren\n");
        exit(EXIT_FAILURE);
    }
    return pFile ;
}

char *leeFileConfig(char *path){
    FILE *pFile = modeOpenFile(path,"r");
    char *linea = (char*)malloc(sizeof(char)*40);
    memset(linea,'\0',40);
    fgets(linea,40,pFile);
    return linea;
}

char *sacaEstacion(char *lineaTren){
    ST_TREN tren;
    char *estacionOrigen = (char*)malloc(sizeof(char)*15);
    char *accion = (char*)malloc(sizeof(char)*15);

    sscanf(lineaTren,"%s%s%d%d%d%s%s",accion,tren.modelo,&tren.infoTren.idTren,&tren.tiempoEspera,&tren.combustible,tren.infoTren.estacionOrigen,tren.infoTren.estacionDestino);
    strcpy(estacionOrigen,tren.infoTren.estacionOrigen);
    free(accion);
    return estacionOrigen;
}

char *obtienePuerto(char *tren,char *ip){
    char *estacionOrigen = sacaEstacion(tren);

    if(strcmp(estacionOrigen,"estacionA")==0){
        strcpy(ip,ipEstacionA);
        return "7400";
    }
    if(strcmp(estacionOrigen,"estacionB")==0){
        strcpy(ip,ipEstacionB);
        return "7800";
    }
    strcpy(ip,ipEstacionC);
    return "8200";
}

bool conoceComando(char *accion){
    return strcmp(accion,"registrar")==0 || strcmp(accion,"solicitarAnden")==0 ||strcmp(accion,"exit")==0;
}

char *procesoAccion (char* accion,char *comando){
    char *auxComando = comando;

    char *path = sacoPath(auxComando);

    if(conoceComando(accion)){
        if(strcmp(accion,"exit")==0){
            return "exit";
        }
        char *tren = leeFileConfig(path);
        return tren;
    }
    else{
        return "desconocido";
    }
}

char *sacoAccion(char *cad,char separador){
    char *auxCad = cad;
    char *acumCad = (char*)malloc(sizeof(char)*LONG_COMANDO+1);
    char *auxAcum = acumCad;

    while(*auxCad && *auxCad != separador){
        *auxAcum = *auxCad;
        auxAcum++;
        auxCad++;
    }
    *auxAcum = '\0';
    return acumCad;
}

char *comando(){
    char *linea = pideComando();
    char *auxLinea = linea;
    char *accion = sacoAccion(auxLinea,' ');

    char *datosTren = procesoAccion(accion,linea);
    if(strcmp(datosTren,"exit")!=0 && strcmp(datosTren,"desconocido")!=0 ){
        char *aux = strcat(accion," ");
        char *tren = strcat(aux,datosTren);
        printf("%s\n",tren);
        return tren;
    }

    return datosTren;
}

int cliente(char *tren,char *puerto,char *IPaConectar){
        int idSocket;
        int activado;
        char msj [50];
        memset(msj,'\0',50);
        struct sockaddr_in direccionServidor;
        direccionServidor.sin_family = AF_INET;
        direccionServidor.sin_addr.s_addr = inet_addr(IPaConectar);//IP a conectar
        //direccionServidor.sin_port = htons(PUERTO);//Puerto a conectar
        direccionServidor.sin_port = htons(atoi(puerto));//Puerto a conectar

        idSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(idSocket < 0){
            printf("No se pudo conseguir el socket\n");
        }

        activado = 1;
        setsockopt(idSocket, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));

        if(connect(idSocket, (void*) &direccionServidor, sizeof(direccionServidor)) != 0){
            perror("No se pudo conectar al servidor\n.");
            return 1;
        }

        send(idSocket,tren,strlen(tren),0);

        if(recv(idSocket,msj,50,0) < 0){
            printf("Error al enviar el tren %s.\n",tren);
        }

        printf("%s",msj);
        close(idSocket);
        printf("-------------------------------------------------------------------------------\n");
        return 0;
}


int main(){

    char *ip = (char*)malloc(sizeof(char)*20);
    char *tren = comando();

    while(strcmp(tren,"desconocido")==0){
            printf("Comando desconocido\n");
            tren = comando();
    }
    if(strcmp(tren,"exit")==0){
        return 0;
    }

    char *puerto = obtienePuerto(tren,ip);

    printf("PUERTO : %s\n",puerto);
    printf("IP : %s\n",ip);

    while(strcmp(tren,"exit")!= 0){

        cliente(tren,puerto,ip);
        tren = comando();

        while(strcmp(tren,"desconocido")==0){
            printf("Comando desconocido\n");
            tren = comando();
        }
        if(strcmp(tren,"exit")==0){
            return 0;
        }
        puerto = obtienePuerto(tren,ip);
    }

    return 0;
}

