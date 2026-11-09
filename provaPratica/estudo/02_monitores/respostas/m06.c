/* m06 — Barreira reutilizável com condição
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção m06.
 * Correção da raiz do projeto:
 * python3 estudo/02_monitores/testes/m06/corrigir.py
 *
 * Leia n,r (1<=n<=16,1<=r<=8). Crie n threads. Em cada rodada j=0..r-1, cada uma registra CHEGA j id, espera todas chegarem e registra PASSA j id. Uma thread pode chegar à próxima rodada após seu próprio PASSA, mesmo se outra ainda não imprimiu PASSA da anterior. Use contador e geração para distinguir rodadas.
 *
 * Exigências: Mutex, condição, while e broadcast. Não reaproveitar contador sem geração. Registro CHEGA é feito com a contagem de chegada; PASSA só após condição da geração satisfeita.
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
void barreira(int rodada, int id);
void *participante(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "m06: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
