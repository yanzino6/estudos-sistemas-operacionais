/* m03 — Buffer com monitor Hoare implementado por semáforos
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção m03.
 * Correção da raiz do projeto:
 * python3 estudo/02_monitores/testes/m03/corrigir.py
 *
 * Leia N,P,C,k (1<=N<=8,1<=P,C<=8,1<=k<=20). Crie P produtores: produtor i insere os valores i*k até (i+1)*k-1. Crie C consumidores e um buffer circular de N posições. Cada item é retirado exatamente uma vez; consumidores imprimem `ITEM valor`. Ordem livre. Use sentinelas -1, uma por consumidor, publicadas pela main somente após joins dos produtores; sentinelas não são impressas. Faça join de todos e termine.
 *
 * Exigências: Implemente entrada/saída, wait e signal de monitor Hoare usando mutex semafórico, fila urgente next e fila por condição; não use pthread_cond_t. Acordado recebe monitor antes do sinalizador continuar. Não substituir por um vetor pronto de resultados. Testes cobrem buffer de uma posição, vários produtores/consumidores e totais não divisíveis por C.
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
    fprintf(stderr, "m03: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
