#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid;

    pid = fork();

    for (int i = 0; i < 2; i++)
        {
            if (pid!=0)
            {
                pid=fork();
            }
        
        }
    
    if (pid==0)
    {
        sleep(2);
        exit(0);
    }
    
    

    else
    {
        int wstatus;
        while (1)
        {   
            pid_t pid_finalizado = waitpid(-1, &wstatus, WNOHANG); //verifica se um filho terminou
                                                                   // se sim, pega seu pid
            sleep(2);
            if (pid_finalizado==-1) // se todos terminaram, sai do loop
            {
                break;
            }
            else if (pid_finalizado==0) // se ninguem terminou, imprime mensagem
            {
                printf("Nenhum filho terminado\n");
            }
            else if (pid_finalizado!=-1) // se alguem terminou, imprime o pid
            {
                printf("Meu filho pid=%d terminou\n",pid_finalizado);
            }
            

        }
        
    }
    

    return  0;
}