#include <stdio.h>
#include <stdlib.h>
#include "../simulador/simulador.h"
#include "../io/io.h" 
#include "../fila/fila.h" 
#include "../processo/processo.h"
#include "../escalonador/escalonador.h"
#include "../constants.h"

static int relogio = 0;
static int totalProcessos = 0;
static int processosFinalizados = 0;
static int quantum = 0;
static int tempoBoostPrioridade = 0;
static const char* NOMES_IO[] = {"DISCO", "FITA", "IMP", "SEM IO"};
static Processo** arrayProcessos = NULL;
static int* linhaTempoCpu = NULL;  
static int tamanhoLinhaTempo = 0; 
static int* processosTerminados = NULL; 
static int tamanhoTerminados = 0; 

static void garantirEspacoArray(int **array, int *tamanho, int instante) {
    if (instante < *tamanho) {
        return;
    }
    
    int novoTamanho = (*tamanho) * 2;
    if ((*tamanho) == 0) {
        novoTamanho = 100;
    }
    
    while (novoTamanho <= instante) {
        novoTamanho *= 2;
    }

    *array = (int*) realloc(*array, sizeof(int) * novoTamanho);
    if (*array == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    
    for (int i = *tamanho; i < novoTamanho; i++) {
        (*array)[i] = 0;
    }
    
    *tamanho = novoTamanho;
}


static void liberarLinhaTempo() {
    if (linhaTempoCpu != NULL) {
        free(linhaTempoCpu);
        linhaTempoCpu = NULL;
        tamanhoLinhaTempo = 0;
    }

    if (processosTerminados != NULL) {
        free(processosTerminados);
        processosTerminados = NULL;
        tamanhoTerminados = 0;
    }
}

static void registrarLinhaTempoCpu(Processo *processo) {
    garantirEspacoArray(&linhaTempoCpu, &tamanhoLinhaTempo, relogio);
    
    if (processo != NULL) {
        linhaTempoCpu[relogio] = processo->pid;
    } else {
        linhaTempoCpu[relogio] = 0;
    }
}

static void registrarFimExecucao(Processo *processo) {
    if (processo == NULL) return;
    
    garantirEspacoArray(&processosTerminados, &tamanhoTerminados, relogio);
    processosTerminados[relogio] = processo->pid;
}

static void gerarLinhaTempoCpu() {
    FILE *arquivo = fopen("linha_tempo_cpu.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo linha_tempo_cpu.txt\n");
        return;
    }

    for (int t = 0; t < relogio; t++) {
        int pid = 0;
        int terminou = 0;
        
        if (t < tamanhoLinhaTempo) {
            pid = linhaTempoCpu[t];
        }
        
        if (t < tamanhoTerminados) {
            terminou = processosTerminados[t];
        }
        
        fprintf(arquivo, "%d %d %d\n", t, pid, terminou);
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

    fprintf(arquivo, "Processo | Ativação | T. Serviço | T. IO | Momento IO| Início CPU | Fim CPU | Turnaround\n");
    fprintf(arquivo, "---------|----------|------------|-------|-----------|------------|---------|-----------\n");

    for (int i = 0; i < total; i++) {
        Processo *p = todosProcessos[i];
        
        const char* nomeIO = NOMES_IO[p->tipoIO];

        int inicioCpu = p->momentoAtivacao;
        int fimCpu = p->momentoFimExecucao;
        int turnaround = fimCpu - inicioCpu;

        fprintf(arquivo, "  P%-4d |   %3d   |    %3d    |  %-5s |    %3d    |    %3d    |     %3d     |  %3d\n", p->pid, p->momentoAtivacao, p->tempoTotal, nomeIO, p->momentoIO, inicioCpu ,fimCpu, turnaround);
    }

    fprintf(arquivo, "------------------------------------------------------------\n");
    fprintf(arquivo, "Informações adicionais:\n");
    fprintf(arquivo, "  - Total de processos: %d\n", total);
    fprintf(arquivo, "  - Quantum utilizado: %d\n", quantum);
    fprintf(arquivo, "  - Tempo total de simulação: %d unidades\n", relogio);

    fclose(arquivo);
    printf("\nRelatório gerado em 'relatorio_processos.txt'\n");
}


int getQuantum() {
    return quantum;
}

void inicializarSimulador(int quantidadeProcessos, int quantumEntrada, int duracaoDisco, int duracaoFita, int duracaoImpressora) {
    relogio = 0;
    totalProcessos = quantidadeProcessos;
    processosFinalizados = 0;
    quantum = quantumEntrada;
    linhaTempoCpu = NULL;
    tamanhoLinhaTempo = 0;
    processosTerminados = NULL;
    tamanhoTerminados = 0;
    bootEscalonador();


    bootDispositivos(duracaoDisco, duracaoFita, duracaoImpressora);
    arrayProcessos = (Processo**) malloc(sizeof(Processo*)*quantidadeProcessos);

    for(int i = 0; i < quantidadeProcessos; i++) {
        arrayProcessos[i] = criarProcesso(criaPid(), 0, quantum);
    }

    //calcula tempo para boost de prioridade
    tempoBoostPrioridade = quantumEntrada * quantidadeProcessos * 5;
}

void executarCiclo() {
    Processo* processoEmExecucao = getProcessoEmExecucao();
    //percorre processos e ativa os processos que forem criados na iteração atual
    for(int i = 0; i < totalProcessos; i++){
        if(arrayProcessos[i]->momentoAtivacao == relogio){
            arrayProcessos[i] -> cpuTimeRestante = quantum;
            admitirProcesso(arrayProcessos[i]);
        }    
    }

    //se nao houver processo na cpu
    if (processoEmExecucao == NULL) {
        iniciaExecucaoNovoProcesso();
        processoEmExecucao = getProcessoEmExecucao();
    }
    
    registrarLinhaTempoCpu(getProcessoEmExecucao());
    //se apos iniciar execucao de novo houver processo na cpu
    if(processoEmExecucao!=NULL){
        //atualiza timers
        processoEmExecucao->tempoDecorrido ++;
        processoEmExecucao->cpuTimeRestante --;

        //se processo acabou
        if (processoEmExecucao->tempoDecorrido == processoEmExecucao->tempoTotal) {
            processoEmExecucao->momentoFimExecucao = relogio;
            registrarFimExecucao(processoEmExecucao);
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
            processoIO->cpuTimeRestante = quantum;
            admitirProcesso(processoIO);
        }
     }
    
    //verifica se está na hora de boost de prioridade - evitar starvation
    if(relogio > 0 && relogio % tempoBoostPrioridade == 0){
        boostPrioridade();
    }

    
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

    

void imprimirResumoFinal() {
    printf("\nResumo final\n");
    printf("Tempo total de simulacao: %d unidade(s)\n", relogio);
    printf("Processos finalizados: %d/%d\n", processosFinalizados, totalProcessos);
}

void liberarMemoria(){
    liberarLinhaTempo(); 

    //liberar memoria escalonador
    liberarMemoriaEscalonador();

    //liberar memoria IO
    liberarMemoriaIO();

    //libera memoria processos
    for(int i = 0; i < totalProcessos; i++){
        destruirProcesso(arrayProcessos[i]);
    }
    free(arrayProcessos);
}
