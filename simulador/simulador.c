#include <stdio.h>
#include <stdlib.h>
#include "simulador.h"
#include "../io/io.h" 
#include "../fila/fila.h" 
#include "../processo/processo.h"
#include "../escalonador/escalonador.h"
#include "../constants.h"

static int relogio = 0;
static int totalProcessos = 0;
static int processosFinalizados = 0;
static int quantum;
static const char* NOMES_IO[] = {"DISCO", "FITA", "IMP", "SEM IO"};
static Processo** arrayProcessos = NULL;
static int linhaTempoCpu[MAX_TEMPO];

static void registrarLinhaTempoCpu(Processo *processo) {
    if (relogio < MAX_TEMPO) {
        if (processo != NULL) {
            linhaTempoCpu[relogio] = processo->pid;
        } else {
            linhaTempoCpu[relogio] = 0; // no caso da cpu ociosa
        }
    }
}

static void gerarLinhaTempoCpu() {
    FILE *arquivo = fopen("linha_tempo_cpu.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo linha_tempo_cpu.txt\n");
        return;
    }

    for (int t = 0; t < relogio; t++) {
        fprintf(arquivo, "%d %d\n", t, linhaTempoCpu[t]);
    }

    fclose(arquivo);
    printf("Arquivo 'linha_tempo_cpu.txt' foi gerado com sucesso\n");
}

static void gerarRelatorioProcessos() {
    FILE *arquivo = fopen("relatorio_processos.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo relatorio_processos.txt\n");
        return;
    }

    Processo **todosProcessos = getTodosProcessos();
    int total = getTotalProcessosEscalonador();

    fprintf(arquivo, "Processo | Ativação | T. Serviço | T. IO | Início CPU | Fim CPU | Turnaround\n");
    fprintf(arquivo, "---------|----------|------------|-------|------------|--------|-------\n");

    for (int i = 0; i < total; i++) {
        Processo *p = todosProcessos[i];
        
        const char* nomeIO = NOMES_IO[p->tipoIO];

        int inicioCpu = p->momentoAtivacao;
        int fimCpu = p->momentoFimExecucao;
        int tempoCpu = fimCpu - inicioCpu;

        fprintf(arquivo, "  P%-4d |   %3d   |    %3d    |  %-5s |    %3d    |   %3d |  %3d\n", p->pid, p->momentoAtivacao, p->tempoTotal, nomeIO, inicioCpu, fimCpu, tempoCpu);
    }

    fprintf(arquivo, "------------------------------------------------------------\n");
    fprintf(arquivo, "Informações adicionais:\n");
    fprintf(arquivo, "  - Total de processos: %d\n", total);
    fprintf(arquivo, "  - Quantum utilizado: %d\n", quantum);
    fprintf(arquivo, "  - Tempo total de simulação: %d unidades\n", relogio);

    fclose(arquivo);
    printf("\nRelatório gerado em 'relatorio_processos.txt'\n");
}


static void atualizarProcessoCpu() {
    
}

void inicializarSimulador(int quantidadeProcessos, int quantumEntrada, int duracaoDisco, int duracaoFita, int duracaoImpressora) {
    relogio = 0;
    totalProcessos = quantidadeProcessos;
    processosFinalizados = 0;
    quantum = quantumEntrada;
    bootEscalonador();


    bootDispositivos(duracaoDisco, duracaoFita, duracaoImpressora);
    arrayProcessos = (Processo**) malloc(sizeof(Processo*)*quantidadeProcessos);

    for(int i = 0; i < quantidadeProcessos; i++) {
        arrayProcessos[i] = criarProcesso(criaPid(), 0, quantum);
    }
}

void executarCiclo() {
    Processo* processoEmExecucao = getProcessoEmExecucao();
    //percorre processos e ativa os processos que forem criados na iteração atual
    for(int i = 0; i < totalProcessos; i++){
        if(arrayProcessos[i]->momentoAtivacao == relogio){
            admitirProcesso(arrayProcessos[i]);
        }    
    }

    //se nao houver processo na cpu
    if (processoEmExecucao == NULL) {
        iniciaExecucaoNovoProcesso();
        processoEmExecucao = getProcessoEmExecucao();
    }
    //se apos iniciar execucao de novo houver processo na cpu
    if(processoEmExecucao!=NULL){
        //atualiza timers
        processoEmExecucao->tempoDecorrido ++;
        processoEmExecucao->cpuTimeRestante --;

        //se processo acabou
        if (processoEmExecucao->tempoDecorrido == processoEmExecucao->tempoTotal) {
            processoEmExecucao->momentoFimExecucao = relogio;
            finalizarProcesso();
            processosFinalizados ++;
            processoEmExecucao = NULL;
        } 
        //processo faz IO
        else if (processoEmExecucao->momentoIO == processoEmExecucao->tempoDecorrido) {
            bloquearProcesso();
            processoEmExecucao = NULL;
        }
        //acabou o quantum
        else if (processoEmExecucao->cpuTimeRestante == 0) {
            aplicaPreempsao();
            processoEmExecucao = getProcessoEmExecucao();
            if (processoEmExecucao != NULL) {
                processoEmExecucao->cpuTimeRestante = quantum;
            }
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

    gerarLinhaTempoCpu();   
    gerarRelatorioProcessos();
    imprimirResumoFinal();
}

void imprimirEstadoSimulador(){}
    

void imprimirResumoFinal() {
    printf("\nResumo final\n");
    printf("Tempo total de simulacao: %d unidade(s)\n", relogio);
    printf("Processos finalizados: %d/%d\n", processosFinalizados, totalProcessos);
}


