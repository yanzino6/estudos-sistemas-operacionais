/* x04 — Partição com vetor de tamanho variável
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção x04.
 * Correção da raiz do projeto:
 * python3 estudo/05_mutex_e_paralelismo/testes/x04/corrigir.py
 *
 * Leia n,t (0<=n<=1000,1<=t<=32) e n inteiros entre -1000000 e 1000000. Divida equilibradamente entre t threads, permitindo intervalos vazios; imprima soma long long. n/t fora do contrato: stdout vazio, retorno 1.
 *
 * Exigências: Tratar n=0, t>n e restos, não dividir antes de validar.
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
void *somar_intervalo(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "x04: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
