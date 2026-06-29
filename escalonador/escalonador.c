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
    if(processoEmExecucao->prioridade < QTD_FILAS){
        processoEmExecucao->prioridade += 1;
    } 
    processoEmExecucao->cpuTimeRestante = QUANTUM;

    //adiciona ele no fim da nova fila de prioridade
    enfileirarProcesso(processoEmExecucao, processoEmExecucao->prioridade);
    processoEmExecucao->status= PRONTO;

    //TODO: Admitir novo processo na CPU
}

void bloquearProcesso(){
    processoEmExecucao -> status = BLOQUEADO;
    //TODO: Enviar para fila de IO correspondente
    //TODO: Admitir novo processo na CPU
}

void finalizarProcesso(){
    processoEmExecucao->status = TERMINOU;
    //TODO: Admitir novo processo na CPU
}

//TODO boostPrioridade

