#ifndef FILA_H
#define FILA_H

//fazendo forward declaration de processo
typedef struct Processo Processo;

typedef struct ElemFila{
    struct ElemFila* proximo;
    Processo* processoP;
    ElemFila* anterior;
}ElemFila;

typedef struct FilaProcessos{
    ElemFila* inicio;
    ElemFila* fim;
}FilaProcessos;

FilaProcessos* criarFila();

void enfileirarProcesso(Processo* processoP, FilaProcessos* filaP);

Processo* desenfileirarProcesso(FilaProcessos* filaP);


Processo* consultarInicioFila(FilaProcessos* filaP);

#endif 