/* p06 — Pipeline de filtro e contagem
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção p06.
 * Correção da raiz do projeto:
 * python3 estudo/03_pipes/testes/p06/corrigir.py
 *
 * Leia n (0<=n<=100), seguido de n linhas de até 100 caracteres. Monte dois filhos e dois pipes: main fornece as linhas a `grep SO`; a saída do grep alimenta `wc -l`. Os programas são executados por execlp e conectados por dup2. Stdout é somente a quantidade de linhas que contêm a substring maiúscula SO. Main recolhe ambos e retorna zero, inclusive quando grep não seleciona linhas. Esta é a versão determinística de treino do exemplo ls | wc.
 *
 * Exigências: Criar ambos antes de enviar/esperar; não usar system ou popen; não contar as linhas na main; fechar todas as escritas para propagar EOF.
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
void executar_grep(int entrada, int saida);
void executar_wc(int entrada);

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    fprintf(stderr, "p06: implemente a solução neste arquivo.\n");
    return 1; /* Troque pelo retorno do seu programa ao implementar. */
}
