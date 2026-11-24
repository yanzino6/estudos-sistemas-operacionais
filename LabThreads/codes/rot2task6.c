#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *routine(void*arg)
{
    
    int *index = (int*)arg;

    printf("Thread [%d]: %d\n");
    
    return NULL;
}

int main () 
{
    pthread_t threads[10];
    for (int i = 0; i < 10; i++)
    {
        /* code */
    }
    

    return 0;
}