/* s08 — Base compartilhada com leitores e escritores
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s08.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s08/corrigir.py
 *
 * Leia R,W,k (1..8 cada). Crie R leitores e W escritores. Cada um acessa a base k vezes. Base inicia em zero; cada escrita soma um. Leitor registra `RI id valor` na entrada e `RF id` na saída; escritor registra `WI id` na entrada e `WF id novo_valor` após incremento, antes de liberar a base. Ids de leitores/escritores são independentes, ambos começam em zero.
 *
 * Exigências: Implemente a catraca R e proteção M/W da Lista 6 usando semáforos. Permitir múltiplos leitores; nenhum escritor junto com qualquer outro acesso. Todas as threads terminam. Mutex de log não deve manter a base serializada durante toda a leitura.
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
#include <semaphore.h>

sem_t mutex_rc;      // Protege o contador de leitores (rc)
sem_t acesso_base;   // Garante exclusão mútua na base (M/W)
sem_t catraca;       // Catraca R para evitar que escritores sofram inanição
sem_t log_mutex;     // Evita que os prints se misturem no terminal

int base=0;
int rc=0;
int wc=0;

int k;

/* Interfaces sugeridas: adapte os argumentos/estruturas à sua organização. */
void *leitor(void *arg)
{   int *num = (int*)arg;
    for (int i = 0; i < k; i++)
    {
    sem_wait(&catraca);
    if (rc==0)
    {
        sem_wait(&acesso_base);
    }
    sem_wait(&mutex_rc);
    rc++;
    sem_post(&mutex_rc);
    sem_post(&catraca);
    printf("RI %d %d\n",*num,base);
    printf("RF %d\n",*num);

    sem_wait(&mutex_rc);
    rc--;
    if (rc==0)
    {
        sem_post(&acesso_base);
    }
    sem_post(&mutex_rc);
    }
    free(num);
    return NULL;
}
void *escritor(void *arg)
{
    int *num = (int*)arg;


    for (int i = 0; i < k; i++)
    {

    sem_wait(&catraca);
    sem_wait(&acesso_base);
    
    base++;
    printf("WI %d\n",*num);
    printf("WF %d %d\n",*num,base);

    sem_post(&acesso_base);
    sem_post(&catraca);
    }
    free(num);
    return NULL;

}

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    int nReaders, nWriters;
    scanf("%d %d %d",&nReaders,&nWriters,&k);
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    pthread_t threadRead[nReaders];
    pthread_t threadWrite[nWriters];
    sem_init(&catraca,0,1);
    sem_init(&acesso_base,0,1);
    sem_init(&mutex_rc,0,1);

    /* TODO 3: criar os participantes e executar o protocolo pedido. */

    for (int i = 0; i < nReaders; i++)
    {
        int *id = malloc(sizeof(int));
        *id=i;
        pthread_create(&threadRead[i],NULL,leitor,id);
    }
    for (int i = 0; i < nWriters; i++)
    {
        int *id = malloc(sizeof(int));
        *id=i;
        pthread_create(&threadWrite[i],NULL,escritor,id);
    }
    for (int i = 0; i < nReaders; i++)
    {
        pthread_join(threadRead[i],NULL);
    }
    for (int i = 0; i < nWriters; i++)
    {
        pthread_join(threadWrite[i],NULL);
    }

    sem_destroy(&catraca);
    sem_destroy(&acesso_base);
    sem_destroy(&mutex_rc);

    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    
    return 0; /* Troque pelo retorno do seu programa ao implementar. */
}
