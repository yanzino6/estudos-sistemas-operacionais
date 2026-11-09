/* t01 — Duas threads com a mesma rotina
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção t01.
 * Correção da raiz do projeto:
 * python3 estudo/04_threads/testes/t01/corrigir.py
 *
 * Leia k (1<=k<=100). Crie duas threads com uma mesma função, argumentos id=0 e id=1. Cada uma imprime `id passo` para passo=0..k-1. Espere ambas. Ordem de linhas livre.
 *
 * Exigências: Duas criações antes dos joins; argumentos distintos, uma linha por printf e sem duplicações.
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
void *rotina(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "t01: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
