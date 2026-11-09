/* p02 — Pipeline ping | grep
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção p02.
 * Correção da raiz do projeto:
 * python3 estudo/03_pipes/testes/p02/corrigir.py
 *
 * Implemente `ping -c 5 google.com | grep rtt` com dois filhos, pipe, dup2 e execlp. Sem entrada; stdout contém apenas linhas selecionadas. Main retorna zero após recolher ambos, inclusive se grep não selecionar linhas. Os testes fornecem ping controlado no PATH, mantendo grep real.
 *
 * Exigências: Sem system/popen; argumentos corretos, ambos criados antes da espera; fechar pontas. Não usar caminho absoluto para ping.
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
void executar_ping(int escrita);
void executar_grep(int leitura);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "p02: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
