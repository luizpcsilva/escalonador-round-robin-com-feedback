#ifndef PROCESSO_H
#define PROCESSO_H

typedef enum{
    NOVO,
    PRONTO,
    BLOQUEADO,
    EXECUCAO,
    TERMINOU
}statusProcesso;

typedef enum{
    BAIXA,
    MEDIA,
    ALTA
}prioridadeProcesso;

typedef enum{
    DISCO,
    FITA_MAGNETICA,
    IMPRESSORA
}tipoIOProcesso;

typedef struct{
    int pid;
    int ppid; //indicação do process id do processo pai
    statusProcesso status;
    prioridadeProcesso prioridade;
    int momentoIO; //momento em que o processo vai parar para fazer IO
    int tempoIO; //tempo que o processo precisa para fazer IO
    tipoIOProcesso tipoIO;
    int cpuTimeRestante; //tempo em execucao restante ate completar um quantum
    int cpuTimeTotal //tempo em execucao total
} Processo;



#endif 