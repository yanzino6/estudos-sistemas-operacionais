/* s07 — Sessões de filme com término sincronizado
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s07.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s07/corrigir.py
 *
 * Leia v (1<=v<=3). Crie 10*v threads fãs distintas e uma thread demonstradora. Cada fã participa exatamente uma vez. Execute v sessões com dez espectadores e não libere saída antes do fim.
 * 
 * Registre uma linha por evento: `ENTRA id` ao reservar/colar; `INICIO` com lote cheio; `FIM` ao terminar a viagem/exibição. Cada participante registra `SAI id` ao sair; só reabra a entrada quando o lote inteiro sair.
 *
 * Exigências: Use semáforos e contadores protegidos. Eventos representam alterações reais do estado, impressos na mesma região crítica da alteração. Sem sleep para garantir ordem; número finito de rodadas e join em todas as threads.
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
void *participante(void *arg);
void *controlador(void *arg);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "s07: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
