/* p01 — Anel pai e dois filhos
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção p01.
 * Correção da raiz do projeto:
 * python3 estudo/03_pipes/testes/p01/corrigir.py
 *
 * Leia um inteiro x entre -1000000 e 1000000. Pai envia ao filho 1, que soma 5 e envia ao filho 2; este soma 10 e devolve ao pai. Imprima `[PARENT] Resultado Final: valor!`.
 *
 * Exigências: Dois filhos irmãos, três pipes, read/write, fechamentos e waitpid; operações nos processos pedidos.
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
void filho1(int entrada, int saida);
void filho2(int entrada, int saida);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "p01: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
