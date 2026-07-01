#include <stdio.h>
#include <stdlib.h>
#include "simulador.h"
#include "../io/io.h" 
#include "../fila/fila.h" 
#include "../processo/processo.h"
#include "../escalonador/escalonador.h"

static int relogio = 0;
static int totalProcessos = 0;
static int processosFinalizados = 0;
static int quantum;

static void atualizarProcessoCpu() {
    
}

void inicializarSimulador(int quantidadeProcessos, int quantumEntrada, int duracaoDisco, int duracaoFita, int duracaoImpressora, int tempoMinServico, int tempoMaxServico) {
    relogio = 0;
    totalProcessos = quantidadeProcessos;
    processosFinalizados = 0;
    quantum = quantumEntrada;
    bootEscalonador();


    bootDispositivos(duracaoDisco, duracaoFita, duracaoImpressora);

    for (int i = 0; i < quantidadeProcessos; i++) {
        admitirProcesso(criarProcesso(criaPid(), 0, quantum));
    }
}

void executarCiclo() {

    //se nao houver processo na cpu
    if (getProcessoEmExecucao() == NULL) {
        iniciaExecucaoNovoProcesso();
    }
    //se apos iniciar execucao de novo houver processo na cpu
    if(getProcessoEmExecucao()!=NULL){
        //atualiza timers
        getProcessoEmExecucao()->tempoDecorrido ++;
        getProcessoEmExecucao()->cpuTimeRestante --;

        //se processo acabou
        if (getProcessoEmExecucao()->tempoDecorrido == getProcessoEmExecucao()->tempoTotal) {
        finalizarProcesso();
        processosFinalizados ++;
        } 
        //processo faz IO
        else if (getProcessoEmExecucao()->momentoIO == getProcessoEmExecucao()->tempoDecorrido) {
            bloquearProcesso();
        }
        //acabou o quantum
        else if (getProcessoEmExecucao()->cpuTimeRestante == 0) {
            aplicaPreempsao();
            getProcessoEmExecucao()->cpuTimeRestante = quantum;
    }
    }

    //atualiza o estado dos processos executando IO
    for(int i = 0; i < QTD_DISPOSITIVOS; i++) { //para cada dispositivo de IO
        Processo* processoIO = dispositivoExecutarUnidade(i);
        //se a funcao retornou processo, entao o IO acabou
        if(processoIO != NULL){
            admitirProcesso(processoIO);
        }
     }

    imprimirEstadoSimulador();
    relogio++;
}

void executarSimulacao() {
    while (processosFinalizados < totalProcessos) {
        executarCiclo();
    }

    imprimirResumoFinal();
}

void imprimirEstadoSimulador(){}
    

void imprimirResumoFinal() {
    printf("\nResumo final\n");
    printf("Tempo total de simulacao: %d unidade(s)\n", relogio);
    printf("Processos finalizados: %d/%d\n", processosFinalizados, totalProcessos);
}


