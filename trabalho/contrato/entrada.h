/* entrada.h (dono: A) - leitura do arquivo de entrada. */
#ifndef ENTRADA_H
#define ENTRADA_H
#include "config.h"
#include "pcb.h"
/* Devolve n (ou -1 se nao abrir, sem imprimir nada). *pcbs = vetor alocado, ordenado por
 * (start_time, pid), com pcb_iniciar ja chamado em cada um.
 * Com -DMONO: num_cpus = 1 e modo_fila = FILA_GLOBAL, qualquer que seja o arquivo. */
int entrada_ler(const char *caminho, Config *cfg, PCB **pcbs);
#endif
