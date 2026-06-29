#include "fila/fila.h"

//forward declaration de processo
typedef struct Processo Processo;

Processo* getProcessoEmExecucao();

void aplicaPreempsao();

void boostPrioridade();