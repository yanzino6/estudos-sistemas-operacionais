/* t06 — Soma dos dez primos em duas threads
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção t06.
 * Correção da raiz do projeto:
 * python3 estudo/04_threads/testes/t06/corrigir.py
 *
 * Sem entrada. Use [2,3,5,7,11,13,17,19,23,29]. Primeira thread soma posições 0..4 e segunda 5..9. Retorne parciais por ponteiros recolhidos com join; main imprime soma total.
 *
 * Exigências: Criar duas antes dos joins; retornar objetos vivos e liberar resultados.
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
void *somar_metade(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "t06: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
