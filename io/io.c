#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "fila.h"

static DispositivoIO* arrayDispositivos[QTD_DISPOSITIVOS];

void bootDispositivos(int duracaoDisco, int duracaoFita, int duracaoImpressora) {
    for (int i = 0; i < QTD_DISPOSITIVOS; i++) {
        arrayDispositivos[i] = (DispositivoIO*) malloc(sizeof(DispositivoIO));
        arrayDispositivos[i]->tipo = (TipoIO) i; 
        
        if (i == IO_DISCO) {
            arrayDispositivos[i]->duracao = duracaoDisco;
        } else if (i == IO_FITA) {
            arrayDispositivos[i]->duracao = duracaoFita;
        } else if (i == IO_IMPRESSORA) {
            arrayDispositivos[i]->duracao = duracaoImpressora;
        }
        
        arrayDispositivos[i]->filaBloqueados = criarFila(); 
    }
}

void dispositivoReceberProcesso(Processo *processo, int tipoIo) {
    if (processo == NULL) return;

    processo->status = BLOQUEADO;  
    processo->tempoIO = arrayDispositivos[tipoIo]->duracao;  
    
    enfileirarProcessos(processo, arrayDispositivos[tipoIo]->filaBloqueados); 
}

void dispositivoExecutarUnidade(int tipoIo) {
    Processo *processo = consultarInicioFila(arrayDispositivos[tipoIo]->filaBloqueados); 

    if (processo != NULL && processo->tempoIO > 0) {  
        processo->tempoIO--;
    }
}

int dispositivoOperacaoTerminou(int tipoIo) {
    Processo *processo = consultarInicioFila(arrayDispositivos[tipoIo]->filaBloqueados); 
    return processo != NULL && processo->tempoIO <= 0;  
}

Processo* dispositivoRetirarConcluido(int tipoIo) {
    if (dispositivoOperacaoTerminou(tipoIo)) {
        return desenfileirarProcesso(arrayDispositivos[tipoIo]->filaBloqueados); 
    }
    return NULL;
}

void imprimirDispositivo(int tipoIo) {
    Processo *processo = consultarInicioFila(arrayDispositivos[tipoIo]->filaBloqueados); 

    printf("%s: ", nomeTipoIo(arrayDispositivos[tipoIo]->tipo)); 
    if (processo == NULL) {
        printf("ocioso");
    } else {
        printf("P%d (%d restante)\n", processo->pid, processo->tempoIO);  
    }
}

void finalizarDispositivos() {
    for (int i = 0; i < QTD_DISPOSITIVOS; i++) {
        if (arrayDispositivos[i] != NULL) {
            free(arrayDispositivos[i]);
            arrayDispositivos[i] = NULL;
        }
    }
}