/* barreira.h (dono: A) - barreira reutilizavel com lider, feita com mutex + cond. */
#ifndef BARREIRA_H
#define BARREIRA_H
#include <pthread.h>
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int total;                  /* participantes */
    int chegaram;               /* quantos ja chegaram nesta geracao */
    unsigned geracao;           /* muda a cada abertura; evita wakeup espurio e permite reuso */
} Barreira;
void barreira_iniciar(Barreira *b, int total);
int  barreira_esperar(Barreira *b);   /* devolve 1 para a ULTIMA thread a chegar (a lider) e 0 para as demais */
void barreira_destruir(Barreira *b);
#endif
