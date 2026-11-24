#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

int vec[10] = {1,2,3,4,5,6,7,8,9,10};

void *soma1(void *arg)
{
    int *soma=malloc(sizeof(int));
    *soma=0;

    for (int i = 0; i < 5; i++)
    {
        *soma +=vec[i];
    }
    
    return (void *)soma;
}

void *soma2(void *arg)
{
    int *soma=malloc(sizeof(int));
    *soma=0;

    for (int i = 5; i < 10; i++)
    {
        *soma +=vec[i];
    }
    
    return (void *)soma;
}

int main ()
{
    int *res[2];
    pthread_t threads[2];

    pthread_create(&threads[0],NULL,soma1,NULL);
    pthread_create(&threads[1],NULL,soma2,NULL);

    for (int i = 0; i < 2; i++)
    {
        pthread_join(threads[i],(void **)&res[i]);
    }
    
    int total = *res[0] + *res[1];

    printf("Soma eh %d\n", total);

    free(res[0]);
    free(res[1]);

    return 0;

}