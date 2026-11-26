/* barreira.h (dono: A) - barreira reutilizavel com lider, sobre pthread_barrier_t. */
#ifndef BARREIRA_H
#define BARREIRA_H
#include <pthread.h>
typedef struct {
    pthread_barrier_t b;        /* barreira POSIX, reutilizavel; count = participantes */
} Barreira;
void barreira_iniciar(Barreira *b, int total);   /* total >= 1 (com 0 o init falha) */
int  barreira_esperar(Barreira *b);   /* devolve 1 para UMA thread por rodada (a lider) e 0 para as demais */
void barreira_destruir(Barreira *b);  /* so quando nenhuma thread esta esperando nela */
#endif
