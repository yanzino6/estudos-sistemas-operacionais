/* t11 — Partilhar heap entre trabalhadores
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção t11.
 * Correção da raiz do projeto:
 * python3 estudo/04_threads/testes/t11/corrigir.py
 *
 * Leia n (0<=n<=1000) e n inteiros entre -10000 e 10000. Aloque vetor no heap. Crie duas threads: a primeira dobra elementos de índice par; a segunda dobra elementos de índice ímpar. Após ambos os joins, main imprime o vetor transformado na ordem original. Cada posição tem um único escritor.
 *
 * Exigências: Compartilhar o mesmo vetor alocado; não copiar todo o vetor para cada thread; main só lê depois dos joins. Sem mutex necessário para posições distintas.
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
void *dobrar_posicoes(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "t11: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
