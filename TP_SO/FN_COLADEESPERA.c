/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

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

#define SUBSTRACFUEL 50
#define DELETED -1


void insertOrdered(ST_NODO **lista,ST_TREN *tren){

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

ST_TREN *deleteFirst(ST_NODO **lista){
    ST_NODO *listaAux = *lista;
    ST_TREN *auxTren = listaAux->tren;
    *lista = (*lista)->ste;
    free(listaAux);
    return auxTren;
}

void sortList(ST_NODO **lista){
    ST_NODO *newList = NULL;
    ST_TREN *tren = NULL;
    while(*lista != NULL){
        tren = deleteFirst(lista);
        insertOrdered(&newList,tren);
    }
    *lista = newList;
}

void subtractFuel(ST_NODO **lista){
    ST_NODO *listaAux = *lista;
    while(listaAux != NULL){
        (listaAux->tren->combustible) -= SUBSTRACFUEL;
        listaAux = listaAux->ste;
    }
}

void showQueue (ST_NODO *lista){
    printf("--------------------------------------------------------------------\n");
    printf("\t\tCOLA DE ESPERA :\n");
    if(lista == NULL){
        printf("COLA DE ESPERA VACIA\n");
        return;
    }
    else{
        ST_NODO *listaAux = lista;
        
        while(listaAux != NULL){
            if(listaAux->tren->combustible < SUBSTRACFUEL){
                printf("POCO COMBUSTIBLE!!\n");
                showTren(listaAux->tren);
            }
            else{
                showTren(listaAux->tren);
            }
            listaAux = listaAux->ste;
        }
    }   
}

ST_TREN *search(ST_NODO **lista,char *nomTren){
    ST_NODO *listaAux = *lista;
    while(listaAux != NULL && strcmp(listaAux->tren->modelo, nomTren)!=0){
        listaAux = listaAux->ste;
    }
    return listaAux->tren;
}

ST_TREN *delete(ST_NODO **lista,char *nomTren){
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

void clearQueue(ST_NODO **lista){
    ST_NODO *nodoAux = NULL;
    while(*lista != NULL){
        nodoAux = *lista;
        *lista = (*lista)->ste;
        free(nodoAux);
    }
}