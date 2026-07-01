#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "simulador/simulador.h"
#include "constants.h"

int main(int argc, char *argv[]) {
    int quantidadeProcessos;
    int quantum;

    if (argc >2){
        quantidadeProcessos = atoi(argv[1]);

        quantum = atoi(argv[2]);

        if(quantidadeProcessos<=0){
            printf("Aviso: Quantidade de processos inválida.");
        }

        if(quantum<=0){
            printf("Aviso: Quantum inválido.");
        }

    }else {
        printf("Nenhum argumento passado. É preciso passar como argumento a quantidade de processos e o quantum.");
        return 1;
    }
    
    srand(10);

    inicializarSimulador(quantidadeProcessos, quantum, DURACAO_DISCO, DURACAO_FITA, DURACAO_IMPRESSORA);
    
    executarSimulacao();
    liberarMemoria();

    return 0;
}