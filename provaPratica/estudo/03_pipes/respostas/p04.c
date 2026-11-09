/* p04 — Ler um pipe até EOF
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção p04.
 * Correção da raiz do projeto:
 * python3 estudo/03_pipes/testes/p04/corrigir.py
 *
 * Leia n (0<=n<=200000). Crie um filho que escreve n bytes x no pipe e fecha sua escrita. Pai fecha sua cópia de escrita, lê até read retornar zero e imprime `BYTES n`. Filho deve escrever em blocos e tratar retornos parciais; pai não pode depender de conhecer n para encerrar a leitura.
 *
 * Exigências: Fechar pontas corretamente e esperar filho. Sem ponta de escrita esquecida. read()==0 é critério de término.
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
void enviar_bytes(int fd, size_t n);
size_t ler_ate_eof(int fd);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "p04: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
