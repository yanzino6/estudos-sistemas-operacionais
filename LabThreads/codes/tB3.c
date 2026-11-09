#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

void *somaPrint(void *arg)
{
    int *val = (int*)arg;
    int *num = malloc(sizeof(int));
    *num=0;
    for (int i = 0; i < 5; i++)
    {
        *num+=primes[i+(5*(*val))];
    }
    printf("Thread [%ld]: %d\n", (long int) pthread_self(), *num);
    free(val);
    return (void *)num;
}

int main () {

    pthread_t threads[2];
    int *res[2];
    for (int i = 0; i < 2; i++)
    {
        int *a=malloc(sizeof(int));
        *a=i;
        pthread_create(&threads[i],NULL, &somaPrint, a);
    }
    for (int i = 0; i < 10; i++)
    {
        pthread_join(threads[i],(void**)&res[i]);
    }
    printf("Total: %d\n", *res[0]+*res[1]);
    free(res[0]);
    free(res[1]);
    
    return 0;
}