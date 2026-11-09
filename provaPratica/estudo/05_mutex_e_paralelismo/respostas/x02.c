/* x02 — Soma com reserva de índice protegida
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção x02.
 * Correção da raiz do projeto:
 * python3 estudo/05_mutex_e_paralelismo/testes/x02/corrigir.py
 *
 * Leia dez inteiros entre -1000000000 e 1000000000. Duas threads retiram índices de um índice global protegido por mutex e somam parciais. Main imprime o total long long após joins.
 *
 * Exigências: mutex_init/lock/unlock/destroy; decisão de limite e reserva indivisíveis; não serializar create/join.
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
void *somar_dinamicamente(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "x02: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
