/* s01 — Cinco blocos ordenados por semáforos
 * MODELO PARA PREENCHER. Este arquivo ainda não resolve o exercício.
 * Enunciado: ../exercicios.md, seção s01.
 * Correção da raiz do projeto:
 * python3 estudo/01_semaforos/testes/s01/corrigir.py
 *
 * Leia k (1<=k<=200). Crie cinco threads. Thread i, com i=0..4, imprime os números de i*k+1 a (i+1)*k, um por linha. Use semáforos para imprimir os blocos em ordem. k=200 reproduz os cinco blocos da aula.
 *
 * Exigências: Quatro semáforos de precedência; cinco creates antes dos joins; cada thread imprime seu bloco.
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

typedef struct {
    int k;
    int i;
    sem_t *semWait;
    sem_t *semFree;
} Parametros;

/* Interfaces sugeridas: adapte os argumentos/estruturas à sua organização. */
void *imprimir_bloco(void *arg)
{
    Parametros *p = (Parametros*)arg;

    if (p->semWait!=NULL)
    {
        sem_wait(p->semWait);
    }
    
    for (int i = (p->i*p->k)+1; i <= (p->i+1)*p->k; i++)
    {
        printf("%d\n",i);
    }
    if (p->semFree!=NULL)
    {
        sem_post(p->semFree);
    }
    return NULL;
}

int main(void) {
    /* TODO 1: ler e validar a entrada descrita no enunciado. */
    /* TODO 2: inicializar recursos e argumentos com vida útil correta. */
    /* TODO 3: criar os participantes e executar o protocolo pedido. */
    /* TODO 4: recolher participantes, liberar recursos e produzir a saída. */
    sem_t sem[4];
    for (int i = 0; i < 4; i++)
    {
        sem_init(&sem[i],0,0);
    }
    int k;
    scanf("%d",&k);
    pthread_t threads[5];
    Parametros p[5];

    for (int i = 0; i < 5; i++)
    {
        
        p[i].i=i;
        p[i].k=k;
        if (i==0)
        {
            p[i].semWait=NULL;
        }
        else
        {
            p[i].semWait=&sem[i-1];
        }
        if (i==4)
        {
            p[i].semFree=NULL;
        }
        else
        {
            p[i].semFree=&sem[i];
        }
        
        pthread_create(&threads[i],NULL,imprimir_bloco,&p[i]);
    }
    for (int i = 0; i < 5; i++)
    {
        
        pthread_join(threads[i],NULL);
    }
    for (int i = 0; i < 4; i++)
    {
        
        sem_destroy(&sem[i]);
    }

    return 0; /* Troque pelo retorno do seu programa ao implementar. */
}
