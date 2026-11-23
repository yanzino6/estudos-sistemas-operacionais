/* fila.h (dono: B) - fila de prontos: lista duplamente encadeada intrusiva, sem travas internas. */
#ifndef FILA_H
#define FILA_H
#include <pthread.h>
#include "pcb.h"
typedef struct {
    PCB *inicio, *fim;
    int tamanho;
    long proximo_seq;
    pthread_mutex_t mutex;
} Fila;
void fila_iniciar(Fila *f);
void fila_destruir(Fila *f);
/* NENHUMA funcao abaixo trava: o chamador ja segura f->mutex. */
void fila_inserir_fim(Fila *f, PCB *p);   /* p->seq = f->proximo_seq++ */
void fila_remover(Fila *f, PCB *p);       /* inicio, meio ou fim, em O(1) */
PCB *fila_remover_inicio(Fila *f);        /* NULL se vazia */
PCB *fila_remover_fim(Fila *f);           /* NULL se vazia (usada na migracao) */
#endif
