/* metricas.h (dono: B) - bloco "=== METRICAS ===". */
#ifndef METRICAS_H
#define METRICAS_H
#include <stdio.h>
#include "pcb.h"
#include "cpus.h"
void metricas_gravar(FILE *saida, const PCB *pcbs, int n, const CPU *cpus, int num_cpus);
#endif
