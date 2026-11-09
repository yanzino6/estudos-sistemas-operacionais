/* s15 — Reserva de recursos limitados
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s15.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s15/corrigir.py
 *
 * Leia n,C,k (1<=C<=n<=16,1<=k<=8). Crie n trabalhadores, cada um usa um dos C recursos idênticos k vezes. Registre `IN id` após adquirir os recursos e `OUT id` antes de liberá-los. Todos devem terminar. O corretor verifica contagem, aquisições/liberações e no máximo C usuários ativos.
 *
 * Exigências: Use semáforos para controlar as aquisições. Crie todos antes dos joins. Os eventos devem corresponder à posse real, não a uma simulação na main.
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
    fprintf(stderr, "s15: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
