/* m01 — Corrigir buffer para vários produtores e consumidores
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção m01.
 * Correção da raiz do projeto:
 * python3 estudo/02_monitores/testes/m01/corrigir.py
 *
 * Leia N,P,C,k (1<=N<=8,1<=P,C<=8,1<=k<=20). Crie P produtores: produtor i insere os valores i*k até (i+1)*k-1. Crie C consumidores e um buffer circular de N posições. Cada item é retirado exatamente uma vez; consumidores imprimem `ITEM valor`. Ordem livre. Use sentinelas -1, uma por consumidor, publicadas pela main somente após joins dos produtores; sentinelas não são impressas. Faça join de todos e termine.
 *
 * Exigências: Use mutex e duas condições; espere em while, nunca if. Cada rotina do monitor realiza uma operação por chamada. Não substituir por um vetor pronto de resultados. Testes cobrem buffer de uma posição, vários produtores/consumidores e totais não divisíveis por C.
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
void monitor_init(int capacidade);
void coloca(int valor);
int retira(void);
void *produtor(void *arg);
void *consumidor(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "m01: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
