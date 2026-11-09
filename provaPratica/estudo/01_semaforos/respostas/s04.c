/* s04 — Pombo correio em lotes de 20
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s04.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s04/corrigir.py
 *
 * Leia v (1<=v<=4), número de viagens. Crie 20*v threads usuárias, cada uma cola uma mensagem de id 0..20*v-1, e uma thread pombo. Cada mensagem aparece uma vez.
 * 
 * Registre uma linha por evento: `ENTRA id` ao reservar/colar; `INICIO` com lote cheio; `FIM` ao terminar a viagem/exibição. O pombo retorna no evento FIM, que libera o lote e permite novas mensagens; não imprima SAI.
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
    fprintf(stderr, "s04: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
