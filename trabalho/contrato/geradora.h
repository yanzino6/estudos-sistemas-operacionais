/* geradora.h (dono: A) - thread geradora (chegadas em lote) e a espera por chegadas. */
#ifndef GERADORA_H
#define GERADORA_H
#include "pcb.h"
void  geradora_iniciar(PCB *pcbs, int n);  /* ANTES de criar qualquer thread: proxima_chegada = pcbs[0].start_time */
void *thread_geradora(void *arg);           /* arg ignorado */
void  esperar_chegadas_ate(int t);          /* bloqueia ate toda chegada com start_time <= t estar na fila */
void  geradora_destruir(void);
#endif
