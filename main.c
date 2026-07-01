#include <stdlib.h>
#include <time.h>
#include "simulador.h"
#include "constants.h"

int main(void) {
    inicializarSimulador(QTD_MAX_PROCESSOS, QUANTUM, DURACAO_DISCO, DURACAO_FITA, DURACAO_IMPRESSORA, TEMPO_MIN_SERVICO, TEMPO_MAX_SERVICO);
    
    executarSimulacao();
    liberarSimulador();

    return 0;
}