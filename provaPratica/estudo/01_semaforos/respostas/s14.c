/* s14 — Buffer circular SPSC sem mutex
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s14.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s14/corrigir.py
 *
 * Leia N,k e k inteiros (1<=N<=8,0<=k<=1000). Use uma thread produtora e uma consumidora. Implemente buffer circular usando somente semáforos vagas/itens: cada índice tem um único escritor. A consumidora imprime cada valor retirado, um por linha, preservando a ordem de entrada.
 *
 * Exigências: Sem mutex no buffer SPSC; publicar item somente após escrever; liberar vaga somente após ler; não imprimir diretamente da entrada.
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
void *produtor(void *arg);
void *consumidor(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "s14: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
