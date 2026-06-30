#include <stdlib.h>
#include <time.h>
#include "simulador.h"

int main(void) {
    int quantidadeProcessos = 6;
    int quantum = 3;
    int duracaoDisco = 4;
    int duracaoFita = 3;
    int duracaoImpressora = 2;
    int tempoMinServico = 5;
    int tempoMaxServico = 12;

    inicializarSimulador(quantidadeProcessos, quantum, duracaoDisco, duracaoFita, duracaoImpressora, tempoMinServico, tempoMaxServico);
    executarSimulacao();
    liberarSimulador();

    return 0;
}