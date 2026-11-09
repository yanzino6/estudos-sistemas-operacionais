#ifndef ESTUDO_SEMAFORO_PORTATIL_H
#define ESTUDO_SEMAFORO_PORTATIL_H
/* Inicialize/destrua na main, antes/depois das threads.
 * Adaptador apenas para threads no mesmo processo. */
#include <semaphore.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
typedef struct {
#ifdef __APPLE__
    sem_t *ptr;
#else
    sem_t objeto;
#endif
} so_sem_t;
static inline sem_t *so_sem_ptr(so_sem_t *s) {
#ifdef __APPLE__
    return s->ptr;
#else
    return &s->objeto;
#endif
}
static inline void so_sem_init(so_sem_t *s, unsigned valor) {
#ifdef __APPLE__
    static unsigned sequencia=0;
    char nome[64];
    do {
        snprintf(nome,sizeof nome,"/so_%ld_%u",(long)getpid(),sequencia++);
        s->ptr=sem_open(nome,O_CREAT|O_EXCL,0600,valor);
    } while(s->ptr==SEM_FAILED && errno==EEXIST);
    if(s->ptr==SEM_FAILED){perror("sem_open");exit(2);}
    if(sem_unlink(nome)==-1){perror("sem_unlink");sem_close(s->ptr);exit(2);}
#else
    if(sem_init(&s->objeto,0,valor)==-1){perror("sem_init");exit(2);}
#endif
}
static inline void so_sem_wait(so_sem_t *s) {
    int r;do{r=sem_wait(so_sem_ptr(s));}while(r==-1 && errno==EINTR);
    if(r==-1){perror("sem_wait");exit(2);}
}
static inline void so_sem_post(so_sem_t *s) {
    if(sem_post(so_sem_ptr(s))==-1){perror("sem_post");exit(2);}
}
static inline void so_sem_destroy(so_sem_t *s) {
#ifdef __APPLE__
    if(sem_close(s->ptr)==-1){perror("sem_close");exit(2);}
#else
    if(sem_destroy(&s->objeto)==-1){perror("sem_destroy");exit(2);}
#endif
}
#endif
