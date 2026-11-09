/* t12 — Quadrados retornados por threads
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção t12.
 * Correção da raiz do projeto:
 * python3 estudo/04_threads/testes/t12/corrigir.py
 *
 * Leia quatro inteiros entre -10000 e 10000. Crie quatro threads, cada uma retorna o quadrado de seu valor em objeto de vida útil válida. Main recolhe e imprime os quatro quadrados na ordem original.
 *
 * Exigências: Quatro creates antes dos joins; recuperar resultados por join, liberar alocações.
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
void *quadrado(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "t12: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
