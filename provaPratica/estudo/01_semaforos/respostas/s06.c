/* s06 — Canibais e cozinheiro com encerramento
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s06.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s06/corrigir.py
 *
 * Leia N,M,k (1<=N<=8,1<=M<=8,1<=k<=8). Travessa começa com M porções. Crie N canibais, cada um come k porções, e um cozinheiro. Registre `COME id` na retirada e `REPOE` ao repor M porções. Só reponha quando vazio e ainda houver refeições pendentes. Encerre o cozinheiro após todas as refeições.
 *
 * Exigências: Semáforos para pedido/retorno, contador protegido; um pedido por reposição; não repor antes de vazio. Ids de 0 a N-1.
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
void *canibal(void *arg);
void *cozinheiro(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "s06: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
