/* cpus.h (dono: B) - estado das CPUs e TODA a diferenca entre GLOBAL e POR_CPU. */
#ifndef CPUS_H
#define CPUS_H
#include <pthread.h>
#include "pcb.h"
typedef struct {
    int indice;
    pthread_t tid;
    /* --- protegidos pela trava de filas (cpus_travar) --- */
    pthread_cond_t cond;        /* par: mutex da fila desta CPU (GLOBAL: o da fila global) */
    int ociosa;                 /* 1 = sem trabalho, aceita entrega. NASCE 1 em cpus_iniciar */
    PCB *entregue;              /* processo escolhido PARA ela por quem a acordou (entrega direta) */
    int t_entrega;              /* instante logico dessa entrega = instante do despacho */
    PCB *executando;            /* processo despachado ou entregue; continua != NULL durante a retencao */
    /* --- so a propria thread escalonadora escreve; main le depois do join --- */
    int ultimo_pid;             /* -1 no inicio */
    int ocupado_ms;             /* soma de fatias x 100 */
    int trocas;                 /* trocas de contexto nesta CPU */
} CPU;
extern CPU *g_cpus;             /* vetor de g_cfg.num_cpus posicoes, criado em cpus_iniciar */

void cpus_iniciar(void);        /* usa g_cfg e g_pol; toda CPU nasce com ociosa = 1 */
void cpus_destruir(void);
/* GLOBAL: trava o mutex da fila global. POR_CPU: trava TODAS as filas, em ordem crescente. */
void cpus_travar(void);
void cpus_destravar(void);
/* ---- as funcoes abaixo exigem cpus_travar() ja feito ---- */
PCB *cpus_tirar_proximo(int k, int t);  /* politica na fila da CPU k; no POR_CPU, com fila vazia, migra
                                           (registra "Migracao" em t). Se devolver p: executando=p e
                                           cpus_executando++ */
void cpus_devolver(int k, PCB *p);      /* fim do despacho: executando=NULL, cpus_executando--;
                                           se p != NULL, reinsere p no fim da fila da CPU k */
void cpus_entregar_ociosas(int t);      /* para cada CPU ociosa, em ordem crescente de indice: escolhe por ela
                                           (cpus_tirar_proximo), guarda em entregue/t_entrega e da signal */
PCB *cpus_esperar_trabalho(int k, int *t); /* dorme ate receber uma entrega (devolve o PCB e poe o instante
                                           em *t) ou ate encerrar (devolve NULL); volta com cpus_travar() feito */
int  cpus_verificar_termino(int t);     /* se terminou: encerrar=1, registra o termino (uma vez) em t,
                                           acorda todas; devolve encerrar */
/* ---- chamada pela geradora SEM nenhuma trava ---- */
void cpus_inserir_lote(PCB *lote, int n, int t, int ultimo);
     /* trava; insere os n processos (ja em ordem de PID; no POR_CPU, cada um na CPU de menor
        carga); registra cada "Chegada" em t; se ultimo, generator_done=1; entrega as ociosas; destrava */
#endif
