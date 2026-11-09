/* x05 — Trylock e sincronização sem sleep
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção x05.
 * Correção da raiz do projeto:
 * python3 estudo/05_mutex_e_paralelismo/testes/x05/corrigir.py
 *
 * Sem entrada. Main inicializa e adquire um mutex de recurso antes de criar uma thread. Trabalhadora tenta pthread_mutex_trylock, confirma EBUSY e imprime OCUPADO. Em seguida avisa main por uma condição protegida por outro mutex. Main só então libera o recurso. Trabalhadora faz lock normal, imprime ADQUIRIDO, libera e termina. Main faz join e destrói os objetos.
 *
 * Exigências: Handshake com mutex de controle distinto do mutex de recurso. Verificar EBUSY, não confundir erro com posse. A thread que adquire o mutex é quem o libera.
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
void *tentar_recurso(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "x05: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
