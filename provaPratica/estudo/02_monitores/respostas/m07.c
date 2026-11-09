/* m07 — Buffer limitado e soma
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção m07.
 * Correção da raiz do projeto:
 * python3 estudo/02_monitores/testes/m07/corrigir.py
 *
 * Leia N,k e k inteiros (1<=N<=16,0<=k<=1000). Uma thread produz os valores num buffer circular e outra os consome somando. Main imprime a soma após joins.
 *
 * Exigências: Mutex e duas condições, waits em while, k=0 termina sem bloqueio.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <pthread.h>
#include "../../_infra/semaforo_portatil.h"

/* Interfaces sugeridas: adapte os argumentos/estruturas à sua organização. */
void *produtor(void *arg);
void *consumidor(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "m07: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
