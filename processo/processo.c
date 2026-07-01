#include <stdio.h>
#include <stdlib.h>
#include "processo.h"


// função que gera o tempo total de um processo, de forma aleatoria, multiplicando o quantum
int calcTempoTotal(int quantum){

    int n = (int) rand() % 8 +1; // o processo sdeve ter no máximo 8 vezes um quantum de tempo total
    int aux = (int) rand() % ((n * quantum +1) +1); // o tamanho do tempo total é o quantum n vezes
    return aux;
}

// função que calcula o tempo necessário para io, de forma aleatoria, com base no tempo total do processo
int calcTempoIO(int tempoTotalProcesso){

    int aux = (int) rand() % (tempoTotalProcesso); // gera um número aleatorio de 0 a tempoTotal -1
    return aux;
}

// função que calcula o momento, de forma aleatoria, em que o processo vai fazer io, caso seja possivel
int calcMomentoIO(int tempoTotalProcesso){
    
    if(tempoTotalProcesso <=1){
        return 0;
    }
    else{int max = tempoTotalProcesso;
        int min = 1; 
        int aux = (int) rand() % (max - min +1) +min; // gera o momento em que o processo vai precisar de io: [1, tempoTotalProcesso-1]
        return aux;
    }
}

// função que gera o pid utilizando um contador de uma variavel estatica
int criaPid(){

    static int pid = 1;
    
    int pidAtual = pid;
    pid++;

    return pidAtual;
}

Processo *criarProcesso(int PID, int PPID, int quantum){ // utilizar no main a função criaPid como parâmetro de criarProcesso
    
    Processo *novoProcesso = (Processo*) malloc(sizeof(Processo)); // aloca espaço para o processo

    if(novoProcesso == NULL){ // verifica se o espaço foi alocado
        return NULL;
    }

    novoProcesso->pid = PID;

    if(novoProcesso->pid == 1){ // se for o primeiro processo, seu pai é o processo 0, se não, aloca o valor passado no parâmetro
        novoProcesso->ppid = 0;
    }
    else{
        novoProcesso->ppid = PPID;
    }

    novoProcesso->tempoDecorrido = 0; // se o processo é novo então ainda não executou
    novoProcesso->status = NOVO;
    novoProcesso->prioridade = 1; // todos os processos iniciam com alta prioridade
    novoProcesso->tempoTotal = calcTempoTotal(quantum); // gera um tempo total do processo com base no quantum
    novoProcesso->tempoIO = calcTempoIO(novoProcesso->tempoTotal); // gera um tempo de io com base no tempo total
    if(novoProcesso->tempoIO == 0){
        novoProcesso->momentoIO = 0;
    }
    else{
        novoProcesso->momentoIO = calcMomentoIO(novoProcesso->tempoTotal); // gera o momento em que o processo vai
                                                                           // parar para fazer io com base no tempo total
    }

    if(novoProcesso->momentoIO == 0){ // se o processo não tem momento io, então não há necessidade de atribuir um tipoIO
        novoProcesso->tipoIO = SEM_IO;
    }
    else{
        novoProcesso->tipoIO = (tipoIOProcesso) rand() % 3;
    }

    return novoProcesso;

}