/* p09 — Soma pelo processo filho
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção p09.
 * Correção da raiz do projeto:
 * python3 estudo/03_pipes/testes/p09/corrigir.py
 *
 * Leia n e n inteiros (0<=n<=1000, valores entre -1000000 e 1000000). Pai envia por pipe; filho soma em long long e devolve por outro pipe; pai imprime só soma.
 *
 * Exigências: Transferências completas, n=0, fechamento e waitpid; soma no filho.
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
void somador(int entrada, int saida);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "p09: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
