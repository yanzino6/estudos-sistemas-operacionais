/* t08 — Dois modos de criação e join
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção t08.
 * Correção da raiz do projeto:
 * python3 estudo/04_threads/testes/t08/corrigir.py
 *
 * Leia modo,n (modo 0 ou 1,1<=n<=16). Modo 0: para cada id, crie e faça join imediatamente; trabalhador imprime INICIO id e FIM id. Modo 1: crie todos antes dos joins; cada trabalhador imprime CHEGA 0 id, aguarda uma barreira de n participantes e imprime PASSA 0 id. A barreira torna verificável a coexistência dos trabalhadores sem medir tempo.
 *
 * Exigências: No modo 0 no máximo um trabalhador do laço vivo. No modo 1 use mutex/condição para barreira; não faça join antes de criar todos.
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
void *trabalhador(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "t08: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
