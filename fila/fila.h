#include "processo/processo.h" 

typedef struct ElemFila{
    ElemFila* proximo;
    Processo* processoP;
}ElemFila;

typedef struct FilaProcessos{
    ElemFila* inicio;
    ElemFila* fim;
}FilaProcessos;

FilaProcessos* criarFila();

void enfileirarProcesso(Processo* processoP, FilaProcessos* filaP);

Processo* desenfileirarProcesso(FilaProcessos);
