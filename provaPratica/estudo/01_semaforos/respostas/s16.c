/* s16 — Contador protegido por semáforo
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s16.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s16/corrigir.py
 *
 * Leia t,k (1<=t<=16,0<=k<=10000). Crie t threads, cada uma incrementa k vezes um contador global usando semáforo binário. Imprima somente o total.
 *
 * Exigências: Semáforo iniciado em 1; criar todos antes dos joins; contador atualizado realmente pelas threads.
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
void *incrementar(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "s16: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
