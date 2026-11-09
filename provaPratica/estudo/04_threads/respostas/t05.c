/* t05 — Retorno de dados alocados pela thread
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção t05.
 * Correção da raiz do projeto:
 * python3 estudo/04_threads/testes/t05/corrigir.py
 *
 * Leia quatro inteiros x entre 0 e 1000000. Cada uma de quatro threads calcula um lançamento determinístico `1 + x % 6`, aloca o resultado e retorna o ponteiro. Main recolhe por join e imprime os quatro resultados na ordem original, liberando cada alocação.
 *
 * Exigências: Resultado alocado dentro da rotina; receber com void** no join; não retornar endereço de variável automática nem compartilhar uma única variável de retorno.
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
void *lancar_dado(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "t05: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
