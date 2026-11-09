/* p07 — Pipe com escrita não bloqueante
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção p07.
 * Correção da raiz do projeto:
 * python3 estudo/03_pipes/testes/p07/corrigir.py
 *
 * Leia n (1<=n<=1000000). Filho escreve n bytes x, em blocos, numa ponta marcada O_NONBLOCK via fcntl. Pai drena o pipe até EOF e acumula número de bytes e soma dos valores dos bytes. Imprima `n soma`, onde x possui valor 120. Ao receber EAGAIN, filho deve aguardar disponibilidade via poll e continuar; tratar também EINTR e writes parciais.
 *
 * Exigências: Não fixar capacidade do pipe nem tratar EAGAIN como perda de dados. Não usar alarm/printf em handler. Resultado deve vir dos bytes recebidos, não de n*120 na main.
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
void escrever_nao_bloqueante(int fd, size_t total);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "p07: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
