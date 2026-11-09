/* t02 — Garantir término antes de sair da main
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção t02.
 * Correção da raiz do projeto:
 * python3 estudo/04_threads/testes/t02/corrigir.py
 *
 * Leia k (1<=k<=100). Crie uma thread trabalhadora que imprime `TRABALHO i` para i=0..k-1. Main só imprime `FIM` depois de todas essas linhas, usando pthread_join.
 *
 * Exigências: Trabalho executado na thread; FIM na main após join. Não usar sleep para esperar.
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
void *trabalho(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "t02: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
