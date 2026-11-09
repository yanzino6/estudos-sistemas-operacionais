/* p03 — Avô, pai e neto
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção p03.
 * Correção da raiz do projeto:
 * python3 estudo/03_pipes/testes/p03/corrigir.py
 *
 * Leia n e n inteiros (1<=n<=100, valores entre -1000000 e 1000000). Avô envia ao filho; filho multiplica por três e envia ao neto; neto inverte e devolve ao filho, que devolve ao avô. Saída: `[GRANDPARENT] Vetor final:` seguida do vetor final.
 *
 * Exigências: Neto criado pelo filho, quatro canais direcionais ou protocolo equivalente; operações nos processos especificados; waitpid na hierarquia.
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
void processo_filho(void);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "p03: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
