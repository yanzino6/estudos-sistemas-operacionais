/* t03 — Argumentos por estrutura
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção t03.
 * Correção da raiz do projeto:
 * python3 estudo/04_threads/testes/t03/corrigir.py
 *
 * Leia quatro pares inicio,fim, com 0<=inicio<=fim<=1000. Crie quatro threads, cada uma recebe uma estrutura com id e intervalo [inicio,fim). Ela soma os inteiros desse intervalo e imprime `id soma`, ordem livre.
 *
 * Exigências: Quatro objetos distintos, preenchidos antes de create e vivos até join. Função recebe void* e converte ao tipo da estrutura.
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
typedef struct { int id, inicio, fim; } Argumento;
void *somar_intervalo(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "t03: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
