/* p08 — FIFO com dois escritores e limpeza
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção p08.
 * Correção da raiz do projeto:
 * python3 estudo/03_pipes/testes/p08/corrigir.py
 *
 * Leia k (1<=k<=100). Crie uma FIFO chamada canal.fifo no diretório de execução e dois filhos escritores. Cada filho envia k registros {origem,sequencia}; pai lê 2*k registros e imprime `origem sequencia`, ordem livre. Use protocolo para que ambos abram antes de começar a escrita e para não confundir o intervalo entre aberturas com fim definitivo. Feche descritores, recolha filhos e remova a FIFO antes de terminar.
 *
 * Exigências: mkfifo com permissões octais, open/read/write/close/unlink. Registros pequenos com escrita atômica e leitura recomposta. Coordenação de abertura, por exemplo canal auxiliar de prontidão, sem sleep. O corretor também verifica que canal.fifo não permaneceu.
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
void escritor_fifo(int origem, int quantidade);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "p08: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
