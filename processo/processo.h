#ifndef PROCESSO_H
#define PROCESSO_H

typedef enum{
    TEMPO_EXECUÇÃO,
    TEMPO_IO
}flagTempo;

typedef enum{
    NOVO,
    PRONTO,
    BLOQUEADO,
    EXECUCAO,
    TERMINOU
}statusProcesso;

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
    int prioridade;
    int tempoTotal; //somatorio do tempo que o processo precisa
    int tempoDecorrido; //tempo dentro de um quantum
    int momentoIO; //momento em que o processo vai parar para fazer IO
    int tempoIO; //tempo que o processo precisa para fazer IO
    tipoIOProcesso tipoIO;
    int momentoAtivacao;
    int cpuTimeRestante; //tempo em execucao restante ate completar um quantum
}Processo;

Processo *criarProcesso(int pid, int ppid, int quantum);

void destruirProcesso(Processo *processo);

void imprimirProcesso(Processo *processo);

void mudarStatusProcesso(Processo *processo, statusProcesso novoStatus);

int calcTempoTotal(int quantum);

int calcTempoIO (int tempoTotalProcesso);

int calcMomentoIO (int tempoTotalProcesso);

int criaPid ();

int calcTempoAtivacao(int quantum);

#endif 