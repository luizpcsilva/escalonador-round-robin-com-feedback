//fazendo forward declaration de processo
typedef struct Processo Processo;

typedef struct ElemFila{
    ElemFila* proximo;
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
