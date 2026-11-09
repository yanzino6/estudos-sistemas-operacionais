/* s01 — Cinco blocos ordenados por semáforos
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s01.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s01/corrigir.py
 *
 * Leia k (1<=k<=200). Crie cinco threads. Thread i, com i=0..4, imprime os números de i*k+1 a (i+1)*k, um por linha. Use semáforos para imprimir os blocos em ordem. k=200 reproduz os cinco blocos da aula.
 *
 * Exigências: Quatro semáforos de precedência; cinco creates antes dos joins; cada thread imprime seu bloco.
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
void *imprimir_bloco(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "s01: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
