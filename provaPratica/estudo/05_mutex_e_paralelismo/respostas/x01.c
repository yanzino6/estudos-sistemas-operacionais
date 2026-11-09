/* x01 — Distribuição dinâmica com índice global seguro
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção x01.
 * Correção da raiz do projeto:
 * python3 estudo/05_mutex_e_paralelismo/testes/x01/corrigir.py
 *
 * Leia n (0<=n<=1000) e n inteiros entre -1000000 e 1000000. Crie quatro threads; elas reservam o próximo índice usando variável global protegida por mutex. Cada reserva imprime `indice valor`, ordem livre. Cada posição deve ser processada exatamente uma vez.
 *
 * Exigências: Teste de limite e incremento na mesma região crítica. Cada trabalhador usa cópia local do índice. Não usar divisão estática: o objetivo é implementar corretamente a reserva global.
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
void *reservar_e_processar(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "x01: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
