/* s12 — Aquisição múltipla tudo-ou-nada
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s12.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s12/corrigir.py
 *
 * Leia m,q (1<=m<=8,1<=q<=100), m contadores iniciais não negativos e q comandos. Cada comando tem letra D ou U seguida de m demandas não negativas. D tenta adquirir o vetor de demandas atomicamente, imprime OK se conseguiu ou FALHA sem mudar nada. U soma o vetor e imprime OK. Ao final imprima `ESTADO` e os m contadores. Todos os valores e somas ficam abaixo de 1000000. Faça down_multiplo/up_multiplo sob um mutex; a main aplica os comandos na ordem da entrada.
 *
 * Exigências: O teste de todos os contadores e a alteração inteira ocorrem na mesma região crítica. Não consumir parcialmente. API deve ser segura para chamadas concorrentes; os casos ordenados tornam o resultado verificável.
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
int down_multiplo(const int *demanda);
void up_multiplo(const int *demanda);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "s12: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
