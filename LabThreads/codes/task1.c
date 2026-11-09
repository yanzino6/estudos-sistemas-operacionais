#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void *imprimeVec(void *arg)
{
    int *val = (int*)arg;
    printf("Thread [%ld]: %d\n", (long int) pthread_self(), primes[(*val)]);
    free(val);
    return NULL;
}

int main () {

    pthread_t threads[10];
    for (int i = 0; i < 10; i++)
    {
        int *a=malloc(sizeof(int));
        *a=i;
        pthread_create(&threads[i],NULL, &imprimeVec, a);
    }
    for (int i = 0; i < 10; i++)
    {
        pthread_join(threads[i],NULL);
    }
    
    return 0;
}