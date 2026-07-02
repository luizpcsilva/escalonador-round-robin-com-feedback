#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

FilaProcessos* criarFila(){
    FilaProcessos* fila = (FilaProcessos*) malloc(sizeof(FilaProcessos));

    fila->inicio = NULL;
    fila->fim = NULL;

    return fila;
}



void enfileirarProcesso(Processo* processoP, FilaProcessos* filaP){
    ElemFila* elementoP = (ElemFila*) malloc(sizeof(ElemFila));
    elementoP->processoP = processoP;
    //caso a fila esteja vazia
    if(filaP->fim == NULL){
        elementoP->proximo = NULL;
        elementoP->anterior = NULL;
        filaP->inicio = elementoP;
        filaP->fim = elementoP;
    }
    //caso a fila tenha elementos
    else{
        elementoP->anterior = filaP->fim;
        filaP->fim->proximo = elementoP;
        elementoP->proximo = NULL;
        filaP->fim = elementoP;
    }
}

Processo* desenfileirarProcesso(FilaProcessos* filaP){
    if(filaP->inicio == NULL) return NULL;
    //atualiza o inicio da fila
    ElemFila* elementoP = filaP->inicio;
    filaP->inicio = elementoP->proximo;
    
    //testa se a fila ainda tem elementos
    if(filaP->inicio != NULL){
        filaP->inicio->anterior = NULL;
    }
    else{
        filaP->fim = NULL; //atualiza o fim caso esteja vazia
    }

    Processo* processoP = elementoP->processoP;
    free(elementoP); // libera a memoria que estava sendo utilizada

    return processoP;
}

Processo* consultarInicioFila(FilaProcessos* filaP){
    if(filaP->inicio == NULL) return NULL;

    return(filaP->inicio->processoP);
}


