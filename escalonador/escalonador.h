#include "fila/fila.h"
#include "../processo/processo.h"

//forward declaration de processo
typedef struct Processo Processo;

Processo* getProcessoEmExecucao();

Processo** getTodosProcessos();

int getTotalProcessosEscalonador();

void bootEscalonador();

//aplica a preempsao caso o quantum acabe ou a fatia de tempo do processo em execução acabe
void aplicaPreempsao();

//bloqueia o processo em execução caso ele solicite IO
void bloquearProcesso();

//lida com o término de um processo durante sua execucao
void finalizarProcesso();

void iniciaExecucaoNovoProcesso();

void boostPrioridade();

void admitirProcesso(Processo* processoP);