#ifndef PROCESSO_H
#define PROCESSO_H
#define QUANTUM 10

typedef enum{
    NOVO,
    PRONTO,
    BLOQUEADO,
    EXECUCAO,
    TERMINOU
}statusProcesso;

typedef enum{
    BAIXA,
    ALTA
}prioridadeProcesso;

typedef enum{
    DISCO,
    FITA_MAGNETICA,
    IMPRESSORA,
    SEM_IO
}tipoIOProcesso;

typedef struct{
    int pid;
    int ppid; //indicação do process id do processo pai
    statusProcesso status;
    prioridadeProcesso prioridade;
    int tempoTotal; //somatorio do tempo que o processo precisa
    int tempoDecorrido; //somatorio do tempo de execução do processo
    int momentoIO; //momento em que o processo vai parar para fazer IO
    int tempoIO; //tempo que o processo precisa para fazer IO
    tipoIOProcesso tipoIO;
} Processo;

Processo *criarProcesso(int pid, int ppid);

void destruirProcesso(Processo *processo);

void imprimirProcesso(Processo *processo);

void mudarStatusProcesso(Processo *processo, statusProcesso novoStatus);

int calcTempoTotal(int quantum);

int calcTempoIO (int tempoTotalProcesso);

int calcMomentoIO (int tempoTotalProcesso);

int criaPid ();

#endif 