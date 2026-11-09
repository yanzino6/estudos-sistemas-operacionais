/* s13 — Agente e fumantes por rodadas
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s13.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s13/corrigir.py
 *
 * Leia q (1<=q<=100), seguido de q pares a,b de ingredientes distintos entre 0,1,2. 0=tabaco,1=papel,2=fósforo. Crie um agente e três fumantes. Em cada rodada, agente disponibiliza o par; só o fumante que possui o ingrediente faltante pode atender. Imprima uma linha `FUMA id` por rodada, na ordem dos pares. Agente espera conclusão antes de publicar outro par; encerre os três fumantes ao final.
 *
 * Exigências: Semáforo individual por fumante e confirmação ao agente; publicar somente para o correto. Main não calcula nem imprime as linhas no lugar das threads.
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
void *agente(void *arg);
void *fumante(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "s13: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
