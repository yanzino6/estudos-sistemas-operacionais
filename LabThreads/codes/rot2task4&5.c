#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

int glob = 0;
int vec[10] = {1,2,3,4,5,6,7,8,9,10};
void *soma(void *arg)
{
    int *i = malloc(sizeof(int));
    *i=0;
    while (1)
    {
        pthread_mutex_lock(&mutex);
        if (glob>10000)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        *i+=glob;
        glob++;
        pthread_mutex_unlock(&mutex);
    }
    return (void*)i;
    
}

int main (){

    int *ret[2];

    pthread_t threads[2];
    for (int i = 0; i < 2; i++)
    {
        pthread_create(&threads[i],NULL,soma,NULL);
    }
    
    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i],(void**)&ret[i]);
    }
    
    printf("Soma 1: %d\nSoma 2: %d\nTotal:%d\n",*ret[0],*ret[1],*ret[0]+*ret[1]);

    pthread_mutex_destroy(&mutex);
    

}