#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main ()
{
    int fdPipe1[2], fdPipe2[2], fdPipe3[2];
    pid_t pid1, pid2;
    int num;
    if (pipe(fdPipe1)!=0||pipe(fdPipe2)!=0)
    {
        perror("Erro na criacao do pipe");
        _exit(1);
    }
    

    pid1 = fork();
    if (pid1<0)
    {
        perror("Erro no fork");
        _exit(1);
    }
    
    if (pid1>0)

    {   
        if (pipe(fdPipe3)!=0)
        {
            perror("Erro na criacao do pipe");
            _exit(1);
        }
        
        pid2 = fork();
        if (pid2<0)
        {
            perror("Erro no fork");
            _exit(1);
        }
        if (pid2>0)
        {
            close(fdPipe1[0]);
            close(fdPipe3[1]);
            close(fdPipe2[1]);
            close(fdPipe2[0]);
            scanf("%d",&num);
            printf("[PROCESSO %d PAI]: Inserindo %d no Pipe 1\n", getpid(),num);
            if (write(fdPipe1[1],&num,4)==-1)
            {
                perror("Erro na escrita processo PAI");
                _exit(1);
            }
            
            close(fdPipe1[1]);
        }
        
    }

    else
    {
        close(fdPipe2[0]);
        close(fdPipe1[1]);
        
        if (read(fdPipe1[0], &num, 4)==-1)
        {
            perror("Erro na leitura do processo filho 1");
            _exit(1);
        }
        
        close(fdPipe1[0]);
        printf("[PROCESSO %d FILHO 1]: Recebi %d do Pipe 1\n",getpid(), num);
        num=num+5;
        printf("[PROCESSO %d FILHO 1]: Inserindo %d no Pipe 2\n", getpid(),num);

        if (write(fdPipe2[1],&num,4)==-1)
        {
            perror("Erro na escrita do processo filho 1");
            _exit(1);
        }
        
        close(fdPipe2[1]);
    }

    if (pid1!=0&&pid2==0)
    {
        close(fdPipe1[1]);
        close(fdPipe1[0]);
        close(fdPipe2[1]);
        close(fdPipe3[0]);

        if (read(fdPipe2[0], &num, 4)==-1)
        {
            perror("Erro na leitura do processo filho 2");
            _exit(1);
        }
        
        close(fdPipe2[0]);
        printf("[PROCESSO %d FILHO 2]: Recebi %d do Pipe 2\n",getpid(), num);
        num=num+10;
        printf("[PROCESSO %d FILHO 2]: Inserindo %d no Pipe 3\n", getpid(),num);

        if (write(fdPipe3[1],&num,4)==-1)
        {
            perror("Erro na escrita do processo filho 2");
            _exit(1);
        }

        close(fdPipe3[1]);
        
    }

    if (pid1>0&&pid2>0)
    {

        if (read(fdPipe3[0], &num, 4)==-1)
        {
            perror("Erro na leitra do processo pai");
            _exit(1);
        }
        

        close(fdPipe3[0]);
        printf("[PROCESSO %d PAI]: Recebi %d do Pipe 3\n", getpid(),num);
    }
    
    

    return 0;

}