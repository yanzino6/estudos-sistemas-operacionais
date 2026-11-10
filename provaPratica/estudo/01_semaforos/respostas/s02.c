/* s02 — Grafo de nove funções
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s02.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s02/corrigir.py
 *
 * Implemente em C/Pthreads o grafo usando semáforos. Crie os trabalhadores com as sequências locais: A: F1,F2,F9,F4; B: F3,F7; C: F8,F5,F6. Cada função é representada por uma linha com seu nome, impressa quando executada. Sem entrada. Cada nó deve aparecer exatamente uma vez. Arestas obrigatórias: F1→F2, F1→F5, F1→F8, F2→F3, F2→F6, F5→F6, F5→F9, F8→F9, F6→F7, F9→F7, F3→F4, F7→F4, F2→F9, F8→F5, F3→F7. Qualquer ordenação que respeite essas dependências é aceita.
 *
 * Exigências: Criar todos os trabalhadores antes dos joins; semáforos inicialmente zero para precedência; sinalizar depois da ação; aguardar antes da dependente. Não imprimir tudo pela main nem serializar todos os trabalhadores com joins intermediários.
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
#include <semaphore.h>
#include "../../_infra/semaforo_portatil.h"

sem_t sems[6];


/* Interfaces sugeridas: adapte os argumentos/estruturas à sua organização. */
void *processo_a(void *arg)
{
    printf("F1\n");
    sem_post(&sems[0]);
    printf("F2\n");
    sem_post(&sems[1]);
    sem_post(&sems[4]);
    sem_wait(&sems[2]);
    printf("F9\n");
    sem_post(&sems[3]);
    sem_wait(&sems[5]);
    printf("F4\n");
    return NULL;
}
void *processo_b(void *arg)
{
    sem_wait(&sems[1]);
    printf("F3\n");
    sem_wait(&sems[3]);
    sem_wait(&sems[3]);
    printf("F7\n");
    sem_post(&sems[5]);
    return NULL;
}
void *processo_c(void *arg)
{
    sem_wait(&sems[0]);
    printf("F8\n");
    printf("F5\n");
    sem_post(&sems[2]);
    sem_wait(&sems[4]);
    printf("F6\n");
    sem_post(&sems[3]);
    return NULL;
}

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    
    for (int i = 0; i < 6; i++)
    {
        sem_init(&sems[i],0,0);
    }
    


    pthread_t threads[3];
    pthread_create(&threads[0],NULL,processo_a,NULL);
    pthread_create(&threads[1],NULL,processo_b,NULL);
    pthread_create(&threads[2],NULL,processo_c,NULL);

    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i],NULL);
    }
    
    for (int i = 0; i < 6; i++)
    {
        sem_destroy(&sems[i]);
    }
    

    return 0; /* Troque pelo retorno do seu programa ao implementar. */
}
