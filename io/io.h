#ifndef IO_H
#define IO_H

#include "../fila/fila.h"

#include "../processo/processo.h" 
#define QTD_DISPOSITIVOS 3 

typedef enum {
    IO_DISCO,      
    IO_FITA,      
    IO_IMPRESSORA  
} TipoIO;

typedef struct DispositivoIO {
    TipoIO tipo;                
    int duracao;                
    FilaProcessos* filaBloqueados; 
} DispositivoIO;

void bootDispositivos(int duracaoDisco, int duracaoFita, int duracaoImpressora);

void dispositivoReceberProcesso(Processo *processo, int tipoIo);

Processo* dispositivoExecutarUnidade(int tipoIo);

int dispositivoOperacaoTerminou(int tipoIo);

Processo* dispositivoRetirarConcluido(int tipoIo);

void imprimirDispositivo(int tipoIo);

void finalizarDispositivos(void);

void liberarMemoriaIO();

const char* nomeTipoIo(TipoIO tipo);

#endif 