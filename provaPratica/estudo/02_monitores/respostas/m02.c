/* m02 — Porto com prioridade de saída em C
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção m02.
 * Correção da raiz do projeto:
 * python3 estudo/02_monitores/testes/m02/corrigir.py
 *
 * Leia N,n (1<=N<=n<=16), capacidade do porto e número de navios. Crie uma thread por navio id=0..n-1. Cada um entra e sai uma vez. Implemente pedido_entrada, entrada_OK, pedido_saida e saida_OK usando monitor. Eventos: PE id registra pedido de entrada; EN id concede entrada e reserva vaga/canal; AT id confirma atracação e libera canal; PS id registra pedido de saída; SA id concede saída e ocupa canal; FI id conclui saída e libera canal/vaga. Imprima cada evento dentro da proteção que altera o estado.
 *
 * Exigências: Apenas um navio no canal; ocupação inclui reservas de entrada; EN não pode ocorrer enquanto há PS ainda não atendido por SA. Um pedido não interrompe travessia já concedida. Use while nos predicados e termine todas as threads.
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
void pedido_entrada(int id);
void entrada_OK(int id);
void pedido_saida(int id);
void saida_OK(int id);
void *navio(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "m02: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
