/* s09 — Barbeiro com semáforos
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s09.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s09/corrigir.py
 *
 * Leia q (1<=q<=100), depois q comandos `A id` (chegada de cliente) ou `T` (autoriza concluir um corte). Ids de clientes são distintos e não negativos. Crie uma thread barbeiro e uma thread despachante (pode ser a main). A despachante só envia o próximo comando após confirmação do anterior. A chegada ocupa a cadeira livre (`ATENDE id`), entra na fila FIFO de cinco lugares (`ESPERA id`) ou é recusada (`RECUSA id`). T sem cliente imprime `LIVRE`; T com cliente imprime `FIM id` seguido de `ATENDE proximo` ou `LIVRE`. Os testes encerram sem clientes pendentes. Esse relógio de comandos torna reproduzível o atendimento concorrente.
 *
 * Exigências: Implemente fila/cadeira usando semáforos. O barbeiro executa as conclusões de corte; despachante envia comandos e aguarda confirmação sem busy wait. Não simular toda a barbearia sequencialmente na main.
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
void *barbeiro(void *arg);
void chegada(int id);
void concluir_corte(void);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "s09: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
