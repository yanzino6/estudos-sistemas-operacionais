// Yan Castello Simmer
// Mat: 2026100407

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main (int argc, char *argv[]) 
{   
    pid_t childpid;
    int n = atoi(argv[1]);

for (int i = 0; i < n; i++)
{
    if ((childpid = fork()) == 0)
    {
        break;
    }
    
}
    printf("Sou um processo\n");

    return 0;
}