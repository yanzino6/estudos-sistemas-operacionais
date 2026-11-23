/* politica.h (dono: A; bonus: B) - politicas de escalonamento atras de uma interface. */
#ifndef POLITICA_H
#define POLITICA_H
#include "config.h"
#include "fila.h"
typedef struct {
    const char *nome;                     /* "FCFS", "RR" ou "PRIORIDADE": vai no log */
    int usa_quantum;                      /* 0 no FCFS */
    PCB *(*escolher)(Fila *f, int cpu);   /* chamador segura f->mutex; remove e devolve o escolhido; NULL se vazia */
} Politica;
const Politica *politica_obter(TipoPolitica t);
extern const Politica *g_pol;             /* definido em main.c */
#endif
