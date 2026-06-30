#include <stdio.h>
#include <stdlib.h>
#include "simulador.h"
#include "io.h" 
#include "fila.h" 
#include "processo.h"

static int relogio = 0;
static int totalProcessos = 0;
static int processosFinalizados = 0;
static Processo **arrayProcessos = NULL;

static FilaProcessos* arrayFilas[QTD_FILAS];

// Estado da CPU
static int quantumPadrao = 0;
static int quantumRestante = 0;

static int sortearIntervalo(int minimo, int maximo) {
    int intervalo = maximo - minimo + 1;
    return minimo + rand() % intervalo;
}

static TipoIO sortearTipoIo(void) {
    return (TipoIO) sortearIntervalo(0, 2); 
}

static void enfileirarProcessoLocal(Processo *processo, TipoFila prioridade) {
    processo->status = PRONTO; 
    processo->prioridade = prioridade;

    enfileirarProcessos(processo, arrayFilas[prioridade]);
}

static void retornarParaEscalonador(Processo *processo, MotivoRetorno motivo) {
    if (processo == NULL) return;

    if (motivo == RETORNO_IO_DISCO || motivo == RETORNO_PREEMPCAO) {
        enfileirarProcessoLocal(processo, FILA_BAIXA);
    } else {
        enfileirarProcessoLocal(processo, FILA_ALTA);
    }
}

static void verificarRetornoIo(int tipoIo) {
    if (!dispositivoOperacaoTerminou(tipoIo)) {
        return;
    }

    Processo *processo = dispositivoRetirarConcluido(tipoIo);
    if (processo == NULL) return;
    
    processo->tempoIO = 0;  
    printf("  P%d concluiu %s\n", processo->pid, nomeTipoIo(tipoIo));  
    
    retornarParaEscalonador(processo, (MotivoRetorno)tipoIo);
}

static void enviarParaIo(Processo *processo) {
    dispositivoReceberProcesso(processo, processo->tipoIO);  
    printf("  Bloqueio: P%d solicitou %s.\n", processo->pid, nomeTipoIo(processo->tipoIO));  
}

static void atualizarProcessoCpu() {
    Processo* processoAtualCpu = getProcessoEmExecucao();
    if (processoAtualCpu == NULL) return;
    
    executarProcesso(processoAtualCpu);
    quantumRestante--;
    
    printf("  CPU: P%d [%d/%d] Q:%d\n", processoAtualCpu->pid, processoAtualCpu->tempoDecorrido, processoAtualCpu->tempoTotal, quantumRestante);

    if (processoFinalizado(processoAtualCpu)) {
        finalizarProcesso();
    } else if (processoDeveSolicitarIO(processoAtualCpu)) {
        bloquearProcesso();
    } else if (quantumRestante <= 0) {
        aplicarPreempsao();
    }
}

void inicializarSimulador(int quantidadeProcessos, int quantum, int duracaoDisco, int duracaoFita, int duracaoImpressora, int tempoMinServico, int tempoMaxServico) {
    relogio = 0;
    totalProcessos = quantidadeProcessos;
    processosFinalizados = 0;
    quantumPadrao = quantum;
    quantumRestante = 0;
    bootEscalonador();

    arrayProcessos = (Processo **) malloc(sizeof(Processo *) * quantidadeProcessos);

    bootDispositivos(duracaoDisco, duracaoFita, duracaoImpressora);

    for (int i = 0; i < quantidadeProcessos; i++) {
        arrayProcessos[i] = criarProcesso(i + 1, 0);
        retornarParaEscalonador(arrayProcessos[i], RETORNO_IO_FITA); 
    }
}

void executarCiclo() {
    printf("\nTempo %d\n", relogio);

    //atualiza o estado dos processos executando IO
    for(int i = 0; i < QTD_DISPOSITIVOS; i++) { 
        dispositivoExecutarUnidade(i);
        verificarRetornoIo(i);
    }

    //atualiza o estado do processo na
    if (processoAtualCpu == NULL) {
        for(int i = 0; i < QTD_FILAS; i++) {
            Processo* p = desenfileirarProcesso(arrayFilas[i]);
            if (p != NULL) {
                processoAtualCpu = p;
                processoAtualCpu->status = EXECUCAO;  
                quantumRestante = quantumPadrao;
                
                if (i == FILA_ALTA) {
                    printf("  Escalonamento: P%d - entrou na CPU vindo da fila Alta.\n", processoAtualCpu->pid);
                } else {
                    printf("  Escalonamento: P%d - entrou na CPU vindo da fila Baixa.\n", processoAtualCpu->pid);
                }
                break;
            }
        }
    }

    atualizarProcessoCpu();

    imprimirEstadoSimulador();
    relogio++;
}

void executarSimulacao() {
    printf("PROCESSOS CRIADOS:\n");
    for (int i = 0; i < totalProcessos; i++) {
        Processo *p = arrayProcessos[i];
        printf("  P%d - Total: %d - IO: %s em %d\n", p->pid, p->tempoTotal, nomeTipoIo(p->tipoIO), p->momentoIO);
    }
    printf("\n");

    while (processosFinalizados < totalProcessos) {
        executarCiclo();
    }

    imprimirResumoFinal();
}

void imprimirEstadoSimulador() {
    printf("PRONTOS - ");

    for (int i = 0; i < QTD_FILAS; i++) {
        if (i == FILA_ALTA) {
            printf("Alta: ");
        } else {
            printf("Baixa: ");
        }

        ElemFila* atual = arrayFilas[i]->inicio;

        if (atual == NULL) {
            printf("-- ");
        } else {
            while (atual != NULL) {
                printf("P%d ", atual->processoP->pid);
                atual = atual->proximo;
            }
        }
    }

    printf("- DISPOSITIVOS - ");
    for(int i = 0; i < QTD_DISPOSITIVOS; i++) {
        imprimirDispositivo(i);
        if (i < QTD_DISPOSITIVOS - 1) {
            printf(" - ");
        }
    }
    
    printf(" - Fim: %d/%d\n", processosFinalizados, totalProcessos);
}

void imprimirResumoFinal() {
    printf("\nResumo final\n");
    printf("Tempo total de simulacao: %d unidade(s)\n", relogio);
    printf("Processos finalizados: %d/%d\n", processosFinalizados, totalProcessos);
}

void liberarSimulador() {
    for (int i = 0; i < QTD_FILAS; i++) {
        // TODO: liberarFila(arrayFilas[i]);
    }

    for (int i = 0; i < totalProcessos; i++) {
        destruirProcesso(arrayProcessos[i]);
    }
    free(arrayProcessos);

    finalizarDispositivos();
}
//TODO Função que soma 1 no tempo do processo em execução 
//(possivel obte-lo na função getProcessoEmExecucao do escalonador)
//e verifica se naquele instante o processo pede I/O ou termina. Se
//sim, mudar estado do processo e inserir processo na fila do escalonador (ainda n fiz essa funcao)
//Essa função vai ser chamada a cada iteração do loop principal

//A cada iteraçao do loop principal, tambem deve verificar se um IO da fila de
//IO acabou. Nesse caso, deve enviar o processo p fila do escalonador usando a mesma funcao acima
