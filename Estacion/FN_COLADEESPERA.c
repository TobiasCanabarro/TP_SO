/* 
 * File:   FN_COLADEESPERA.c
 * Author: tobias-pc
 * 
 * Created on 25 de noviembre de 2019, 18:33
 */

#include <stdio.h>
#include <string.h>  
#include <stdlib.h>
#include <stdbool.h>

#include "FN_COLADEESPERA.h"
#include "ESTRUCTURA.h"
#include "FN_ESTACION.h"
#include "FN_TRENES.h"
#include "FN_COMANDOS.h"

#define SUBSTRACFUEL 20
#define DELETED -1

void insertOrdered(ST_NODO **lista, ST_TREN *tren){

    ST_NODO *nodo = (ST_NODO*)malloc(sizeof(ST_NODO));
    nodo->tren = (ST_TREN*)malloc(sizeof(ST_TREN));
    (*nodo->tren) = *tren;
    nodo->ste = NULL;
    
    ST_NODO *listaAux = *lista;
    ST_NODO *nodoAnt = NULL;
    
    while(listaAux != NULL && (tren->combustible + tren->tiempoEspera) < 
            (listaAux->tren->combustible + listaAux->tren->tiempoEspera)){
        nodoAnt = listaAux;
        listaAux = listaAux->ste;
    }
    
    if(nodoAnt == NULL){
        *lista = nodo;
    }else{
        nodoAnt->ste = nodo;
    }
    nodo->ste = listaAux;
}

void subtractFuel(ST_ESTACION *estacion){
    if(estacion->ocupado == true){
       estacion->ocupaAnden.combustible -= SUBSTRACFUEL;
       return;
    }
    else{
        ST_NODO *listaAux = estacion->colaDeEspera;
        while(listaAux != NULL){    
            (listaAux->tren->combustible) -= SUBSTRACFUEL;
            listaAux = listaAux->ste;
        }
    }
}

void showQueue (ST_NODO *lista){
    printf("--------------------------------------------------------------------\n");   
    if(lista == NULL){
        printf("\t\tESTADO DE LA COLA DE ESPERA : VACIA\n");
        return;
    }
    else{
        printf("\t\tESTADO DE LA COLA DE ESPERA :\n");
        ST_NODO *listaAux = lista;
        
        while(listaAux != NULL){
            if(listaAux->tren->combustible <= SUBSTRACFUEL){
                printf("\n");
                printf("Modelo : %s\n ID del Tren : %d\n Combustible (POCO COMBUSTIBLE!) : %d L\n Tiempo de Espera : %d m\n Estacion Origen : %s\n Estacion Destino : %s\n PID : %d\n Estado Del Tren : %s\n",
                listaAux->tren->modelo, listaAux->tren->infoTren.idTren, listaAux->tren->combustible, listaAux->tren->tiempoEspera,
                listaAux->tren->infoTren.estacionOrigen, listaAux->tren->infoTren.estacionDestino, listaAux->tren->pID, listaAux->tren->estado);                   
            }
            else{
                showTren(listaAux->tren);
            }
            listaAux = listaAux->ste;
        }
    }   
}


ST_TREN *searchInQueue(ST_NODO *lista,char *nomTren){
    ST_NODO *listaAux = lista;
    while(listaAux != NULL && (strcmp(listaAux->tren->modelo, nomTren)!=0)){
        listaAux = listaAux->ste;
    }
    return listaAux->tren;
}

ST_TREN *deleteTrenInQueue(ST_NODO **lista, char *nomTren){
    ST_NODO *listaAux = *lista;
    ST_NODO *nodoAnt = NULL;
    
    while(listaAux != NULL && strcmp(listaAux->tren->modelo, nomTren) != 0){
        nodoAnt  = listaAux;
        listaAux = listaAux->ste;
    }
    if(nodoAnt == NULL ){
        *lista = (*lista)->ste;
    }else{
        nodoAnt->ste = listaAux->ste;
    }
    ST_TREN *tren = (listaAux->tren);
    free(listaAux);
    
    return tren;
}


