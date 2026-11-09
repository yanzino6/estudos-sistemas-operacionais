/* x03 — Soma de mil elementos com N threads
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção x03.
 * Correção da raiz do projeto:
 * python3 estudo/05_mutex_e_paralelismo/testes/x03/corrigir.py
 *
 * Leia t (1<=t<=1000), seguido de exatamente 1000 inteiros entre -1000000 e 1000000. Divida em intervalos de tamanhos que diferem no máximo em um. Use argumentos alocados dinamicamente e colete parciais com join. Imprima soma long long. t inválido: stdout vazio, retorno 1.
 *
 * Exigências: Cobrir todas as posições sem sobreposição, tratar resto, liberar argumentos/resultados, criar todos antes dos joins.
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
    fprintf(stderr, "x03: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
