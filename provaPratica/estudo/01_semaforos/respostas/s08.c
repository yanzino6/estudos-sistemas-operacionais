/* s08 — Base compartilhada com leitores e escritores
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s08.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s08/corrigir.py
 *
 * Leia R,W,k (1..8 cada). Crie R leitores e W escritores. Cada um acessa a base k vezes. Base inicia em zero; cada escrita soma um. Leitor registra `RI id valor` na entrada e `RF id` na saída; escritor registra `WI id` na entrada e `WF id novo_valor` após incremento, antes de liberar a base. Ids de leitores/escritores são independentes, ambos começam em zero.
 *
 * Exigências: Implemente a catraca R e proteção M/W da Lista 6 usando semáforos. Permitir múltiplos leitores; nenhum escritor junto com qualquer outro acesso. Todas as threads terminam. Mutex de log não deve manter a base serializada durante toda a leitura.
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
void *leitor(void *arg);
void *escritor(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "s08: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
