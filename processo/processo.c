#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

// função que gera o tempo total de um processo, de forma aleatoria, multiplicando o quantum
int calcTempo(int quantum, flagTempo flag){

    // se a flag for 0, então estamos calculando o tempo de cpu que não pode ser 0
    if( flag == TEMPO_EXECUCÃO){
        int n = (int) rand() % 8 +1; // o processo sdeve ter no máximo 8 vezes um quantum de tempo total
        int aux = (int) rand() % (n * quantum +1) +1; // o tamanho do tempo total é o quantum n vezes
        return aux;
    } // se a flag for diferente, então temos que calcular o tempo de io que pode ser 0
    else{
        int n = (int) rand() % 8 +1; // o processo sdeve ter no máximo 8 vezes um quantum de tempo de io
        int aux = (int) rand() % (n * quantum +1); // o tamanho do tempo de io é o quantum n vezes
        return aux;
    }
}

/* função que calcula o tempo necessário para io, de forma aleatoria, com base no tempo total do processo
int calcTempoIO(int tempoTotalProcesso, int quantum){

    int aux = (int) rand() % (tempoTotalProc); // gera um número aleatorio de 0 a tempoTotal -1
    return aux;
}
*/

// função que calcula o momento, de forma aleatoria, em que o processo vai fazer io, caso seja possivel
int calcMomentoIO(int tempoTotalProcesso){
    
    if(tempoTotalProcesso <=1){
        return -1;
    }
    else{
        int max = tempoTotalProcesso -1;
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

// 
void mudarStatusProcesso(Processo *processo, statusProcesso novoStatus){
    processo->status = novoStatus;
    return;
}

// acaba com o processo limpando a memória dele
void destruirProcesso(Processo* processo){
    free(processo);
    return;
}


// função que imprime um processo utilizando vetores de strings para imprimir status e tipoIO
void imprimirProcesso(Processo *processo) {
    if (!processo) {
         printf("Sem Processo\n"); 
         return; 
    }
    // vetores de status e tipo de io para imprimir
    const char *status[] = {"NOVO","PRONTO","BLOQUEADO","EXECUCAO","TERMINOU"};
    const char *io[] = {"DISCO","FITA_MAGNETICA","IMPRESSORA","SEM_IO"};

    // utiliza como indice dos vetores o numero atribuido a status e tipoIO 
    printf("PID:%d PPID:%d Status:%s Prior:%d Tot:%d Dec:%d IO_mom:%d IO_tempo:%d IO_tipo:%s CPU_rest:%d\n",
           processo->pid, processo->ppid, status[processo->status], processo->prioridade,
           processo->tempoTotal, processo->tempoDecorrido, processo->momentoIO, processo->tempoIO,
           io[processo->tipoIO], processo->cpuTimeRestante);
    
    return;
}

int calcTempoAtivacao(int quantum){

    int n = 3;
    int aux = (int) rand() % (n*quantum); // calcula o tempo de ativação sendo entre 0 e n x quantum -1
    return aux;

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

    /*
        como o tempo de io pode ser maior do que o tempo de execução e vice versa 
        então faz sentido utilizar a mesma função para calcular o tempo dos dois.
        a flag inicia com 0, pois primeiro calculamos o tempo de execução
    */

    flagTempo flag= TEMPO_EXECUCÃO;
    novoProcesso->tempoTotal = calcTempo(quantum, flag); // gera o tempo de execução do processo com base no quantum

    flag = TEMPO_IO; 
    novoProcesso->tempoIO = calcTempo(quantum, flag); // gera um tempo de io com base no quantum

    if(novoProcesso->tempoIO == 0){
        novoProcesso->momentoIO = -1;
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
    novoProcesso->momentoAtivacao = calcTempoAtivacao(quantum);
    novoProcesso->cpuTimeRestante = quantum;
    novoProcesso->momentoFimExecucao = 0;

    return novoProcesso;

}