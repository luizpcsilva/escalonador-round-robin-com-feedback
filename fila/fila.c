#include "fila.h"
#include <stdlib.h>

FilaProcessos* criarFila(){
    FilaProcessos* fila = (FilaProcessos*) malloc(sizeof(FilaProcessos));

    fila->inicio = NULL;
    fila->fim = NULL;
}

void enfileirarProcessos(Processo* processoP, FilaProcessos* filaP){
    ElemFila* elementoP = (ElemFila*) malloc(sizeof(ElemFila));
    elementoP->processoP = processoP;
    //caso a fila esteja vazia
    if(filaP->fim == NULL){
        elementoP->proximo = NULL;
        filaP->inicio = elementoP;
        filaP->fim = elementoP;
    }
    //caso a fila tenha elementos
    else{
        filaP->fim->proximo = elementoP;
        elementoP->proximo = NULL;
        filaP->fim = elementoP
    }

Processo* desenfileirarProcesso(FilaProcessos){
    
}



}