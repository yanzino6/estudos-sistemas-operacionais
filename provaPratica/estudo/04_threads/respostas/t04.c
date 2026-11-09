/* t04 — Corrigir passagem de &i
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção t04.
 * Correção da raiz do projeto:
 * python3 estudo/04_threads/testes/t04/corrigir.py
 *
 * Leia dez inteiros. Crie dez threads, cada uma imprime `indice valor`, ordem livre. Cada argumento deve ser alocado individualmente com malloc, preenchido antes de create e liberado pela thread após copiar seus dados. Use índice 0..9.
 *
 * Exigências: Não passar endereço da variável de controle; dez mallocs distintos, tratar falhas e fazer todos os joins.
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
void *imprimir_valor(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "t04: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
