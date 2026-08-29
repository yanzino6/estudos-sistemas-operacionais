// Yan Castello Simmer
// Mat: 2026100407

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//Tarefa: Montar árvore de processos

int main (int argc, char *argv[]) 
{   
    pid_t childpid, c1, c2;
    c2 = 0;
    c1 = fork () ; /* fork number 1 */
    if ( c1 == 0)
        {c2 = fork ();}  /* fork number 2 */
    fork () ; /* fork number 3 */
    if ( c2 > 0)
        {fork () ;}
    printf("Sou um processo\n");

    return 0;
}