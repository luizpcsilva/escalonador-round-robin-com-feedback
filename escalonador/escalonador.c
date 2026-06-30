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

void iniciaExecucaoNovoProcesso(){
    processoEmExecucao = NULL;
    //varre as listas de prioridade de maior para menor até encontrar um processo
    for(int i = 0; i < QTD_FILAS; i++){
        Processo* processoP = desenfileirarProcesso(arrayFilas[i]);
        
        //se a fila está vazia, avança no for
        if(processoP == NULL){
            continue;
        }
        processoEmExecucao = processoP;
        processoEmExecucao->status = EXECUCAO;
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
    
    iniciaExecucaoNovoProcesso();
}

void bloquearProcesso(){
    processoEmExecucao -> status = BLOQUEADO;
    //TODO: Enviar para fila de IO correspondente
    iniciaExecucaoNovoProcesso();
}

void finalizarProcesso(){
    processoEmExecucao->status = TERMINOU;
    destruirProcesso(processoEmExecucao);
    iniciaExecucaoNovoProcesso();
}

void admitirProcesso(Processo* processoP){
    processoP->status = PRONTO;
    processoP->prioridade = 0;
    enfileirarProcesso(processoP, 0);

}

//TODO boostPrioridade

