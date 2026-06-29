#include "escalonador.h"
#include "processo/processo.h"
#include "fila/fila.h"
#include <stdlib.h>
#include "../constants.h"

static Processo* processoEmExecucao = NULL;
static FilaProcessos* arrayFilas[QTD_FILAS];


Processo* getProcessoEmExecucao(){
    return processoEmExecucao;
}

void bootEscalonador(){
    for(int i = 0; i < QTD_FILAS; i++){
        arrayFilas[i] = criarFila();
    }
}

void aplicaPreempsao(){
    
}

//TODO boostPrioridade

