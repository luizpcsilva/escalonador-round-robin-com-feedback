#ifndef SIMULADOR_H
#define SIMULADOR_H

#define QTD_FILAS 2 

typedef enum {
    FILA_BAIXA = 0,
    FILA_ALTA = 1
} TipoFila;

typedef enum {
    RETORNO_IO_DISCO = 0,
    RETORNO_IO_FITA = 1,
    RETORNO_IO_IMPRESSORA = 2,
    RETORNO_PREEMPCAO = -1
} MotivoRetorno;

void inicializarSimulador(int quantidadeProcessos, int quantum, int duracaoDisco, int duracaoFita, int duracaoImpressora, int tempoMinServico, int tempoMaxServico);

void executarSimulacao(void);

void imprimirEstadoSimulador(void);

void imprimirResumoFinal(void);

void liberarSimulador(void);

#endif 