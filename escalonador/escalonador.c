#include "escalonador.h"
#include "../processo/processo.h"
#include "fila/fila.h"
#include <stdlib.h>
#include "../constants.h"
#include "../simulador/simulador.h"
#include "../io/io.h"
#include <stdio.h>

static Processo* processoEmExecucao = NULL;
static FilaProcessos* arrayFilas[QTD_FILAS];
static Processo** todosProcessos = NULL;
static int totalProcessosEscalonador = 0;


Processo** getTodosProcessos() {
    return todosProcessos;
}

FilaProcessos** getArrayFilas(){
    return arrayFilas;
}

void salvarEstadoFilas(int relogio) {
    FILE *arquivo = fopen("historico_filas.txt", "a"); // Abre para adicionar (append)
    if (arquivo == NULL) return;

    fprintf(arquivo, "\n[Relógio: %d]\n", relogio);
    
    for (int i = 0; i < QTD_FILAS; i++) {
        fprintf(arquivo, "Fila %d: ", i);
        
        ElemFila* atual = arrayFilas[i]->inicio;
        if (atual == NULL) {
            fprintf(arquivo, "Vazia");
        } else {
            while (atual != NULL) {
                fprintf(arquivo, "[P%d] ", atual->processoP->pid);
                atual = atual->proximo;
            }
        }
        fprintf(arquivo, "\n");
    }
    fprintf(arquivo, "---------------------------\n");
    fclose(arquivo);
}

int getTotalProcessosEscalonador() {
    return totalProcessosEscalonador;
}

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
        //checa se é a primeira execução do processo
        if(processoP->tempoDecorrido ==0){
            processoP->momentoInicioExecucao = getRelogio();
        }
        if(processoEmExecucao->cpuTimeRestante==0){
            processoEmExecucao->cpuTimeRestante = getQuantum();
        }
        
        processoEmExecucao->status = EXECUCAO;
        
        break;
    }   
}

void aplicaPreempsao(){
    if(processoEmExecucao->prioridade < QTD_FILAS - 1){
        processoEmExecucao->prioridade += 1;
    }
    //atualiza o tempo de quantum dele
    processoEmExecucao->cpuTimeRestante = getQuantum();
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
    iniciaExecucaoNovoProcesso();
}

void admitirProcesso(Processo* processoP){
    if(processoP->status == NOVO) {
        todosProcessos = (Processo**) realloc(todosProcessos, sizeof(Processo*) * (totalProcessosEscalonador + 1));
        todosProcessos[totalProcessosEscalonador] = processoP;
        totalProcessosEscalonador++;
    }
    
    if(processoP->status == BLOQUEADO && processoP->tipoIO == DISCO){
        processoP->prioridade = QTD_FILAS-1;
    }
    else{
        processoP->prioridade = 0;
    }
    processoP->status = PRONTO;

    enfileirarProcesso(processoP, arrayFilas[processoP->prioridade]);

    // Se há alguém na CPU e o processo recém-chegado tem prioridade maior
    if (processoEmExecucao != NULL && processoP->prioridade < processoEmExecucao->prioridade) {
        //enfileira processo atual e inicia execucao do novo
        processoEmExecucao->status = PRONTO;
        enfileirarProcesso(processoEmExecucao, arrayFilas[processoEmExecucao->prioridade]);
        iniciaExecucaoNovoProcesso();
    }

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

void liberarMemoriaEscalonador(){
    //para cada fila
    for(int i = 0; i < QTD_FILAS; i++){
        //desenfilere até esvaziar a fila
        while(desenfileirarProcesso(arrayFilas[i]) != NULL){}
    }
}

