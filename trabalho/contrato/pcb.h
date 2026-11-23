/* pcb.h (dono: A) - BCP, TCB e a thread trabalhadora (protocolo da fatia). */
#ifndef PCB_H
#define PCB_H
#include <pthread.h>
#include "barreira.h"
typedef enum { READY, RUNNING, FINISHED } ProcessState;
struct TCB;
typedef struct PCB {
    /* ---- campos do enunciado (secao 2.9), mesmos nomes ---- */
    int pid;
    int process_len;            /* duracao total (ms) */
    int remaining_time;         /* tempo restante (ms) */
    int priority;               /* 1 (mais alta) a 5 */
    int num_threads;
    int start_time;             /* chegada (ms) */
    int cpu_id;                 /* CPU atual; -1 se nenhuma */
    int first_dispatch;         /* instante LOGICO do 1o despacho; -1 se ainda nao */
    int finish_time;            /* instante LOGICO da finalizacao; -1 se ainda nao */
    ProcessState state;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
    pthread_t *thread_ids;
    /* ---- extras (justificados no README) ---- */
    Barreira barreira;              /* alinha as threads no fim de cada fatia */
    pthread_cond_t cv_escalonador;  /* lider da fatia -> escalonador; par: mutex */
    int orcamento;                  /* fatias que ainda pode usar neste despacho */
    int fatias_no_despacho;         /* fatias executadas neste despacho */
    int t_despacho;                 /* instante logico do despacho atual */
    int ultima_cpu;                 /* CPU do ultimo despacho (-1 = nunca); usado no bonus */
    struct TCB *tcbs;               /* vetor de num_threads TCBs (liberado no fim) */
    /* ---- no intrusivo da fila: so mexe quem segura o mutex da fila onde o PCB esta ---- */
    struct PCB *ant, *prox;
    long seq;                       /* ordem de entrada na fila (desempate FIFO da prioridade) */
} PCB;
typedef struct TCB { PCB *pcb; int thread_index; } TCB;
#define ORCAMENTO_ILIMITADO 0x3fffffff
void  pcb_iniciar(PCB *p);          /* chamar depois de preencher pid..start_time */
void  pcb_destruir(PCB *p);         /* chamar depois do join das threads do processo */
void *thread_trabalhadora(void *arg_tcb);
#endif
