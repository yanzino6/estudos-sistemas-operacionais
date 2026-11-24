#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

int vec[10] = {0,1,2,3,4,5,6,7,8,9};

void *printaIndice(void *arg)
{
    int *i=(int * )arg;

    printf("Thread [%ld]: %d\n", pthread_self(), vec[*i]);
    free(i);
    return NULL;
}


int main()
{

    pthread_t threads[10];
    for (int i = 0; i < 10; i++)
    {
        int *indice = malloc(sizeof(int));
        *indice=i;
        pthread_create(&threads[i],NULL,printaIndice,indice);
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}