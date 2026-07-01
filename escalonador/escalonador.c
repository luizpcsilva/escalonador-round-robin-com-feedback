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
        break;
    }   
}

void aplicaPreempsao(){
    if(processoEmExecucao->prioridade < QTD_FILAS){
        processoEmExecucao->prioridade += 1;
    } 

    //adiciona ele no fim da nova fila de prioridade
    enfileirarProcesso(processoEmExecucao, arrayFilas[processoEmExecucao->prioridade]);
    processoEmExecucao->status= PRONTO;
    
    iniciaExecucaoNovoProcesso();
}

void bloquearProcesso(){
    processoEmExecucao -> status = BLOQUEADO;
    dispositivoReceberProcesso(processoEmExecucao, processoEmExecucao->tipoIO);  
    iniciaExecucaoNovoProcesso();
}

void finalizarProcesso(){
    processoEmExecucao->status = TERMINOU;
    destruirProcesso(processoEmExecucao);
    iniciaExecucaoNovoProcesso();
}

void admitirProcesso(Processo* processoP){
    if(processoP->status == BLOQUEADO && processoP->tipoIO == DISCO){
        processoP->prioridade = QTD_FILAS-1;
    }
    else{
        processoP->prioridade = 0;
    }
    processoP->status = PRONTO;
    
    enfileirarProcesso(processoP, arrayFilas[0]);
}

void boostPrioridade(){
    //seleciona todos os processos das filas de prioridade
    //menor que 0 e os move para fila de maior prioridade
    for(int i=1; i < QTD_FILAS; i++){
        Processo* processoP = desenfileirarProcesso(arrayFilas[i]);
        //enquanto a fila nao estiver vazia
        while(processoP != NULL){
            admitirProcesso(processoP);
            processoP = desenfileirarProcesso(arrayFilas[i]);
        }
    }
}

