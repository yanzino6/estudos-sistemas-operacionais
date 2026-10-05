#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main() 
{

    int fdPipe[2];
    pid_t pid1, pid2;
    int tamvec;
    printf("[ GRANDPARENT ] Digite o tamanho do vetor : ");
    scanf("%d", &tamvec);
    int vec[tamvec];
    
    if (pipe(fdPipe)!=0)
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
        printf("[ GRANDPARENT ] Digite os %d elementos : ", tamvec);

        for (int i = 0; i < tamvec; i++)
        {
            scanf("%d", &vec[i]);
        }
        if (write(fdPipe[1],vec,tamvec*sizeof(int))==-1)
        {
            perror("Erro na escrita do processo GRANDPARENT");
            _exit(1);
        }
        close(fdPipe[1]);

        if (waitpid(pid1, NULL, 0)==-1)
        {
            perror("Erro na espera do processo PARENT");
            _exit(1);
        }

        if (read(fdPipe[0],vec,tamvec*sizeof(int))==-1)
        {
            perror("Erro na leitura do processo GRANDPARENT");
            _exit(1);
        }
        close(fdPipe[0]);
        
        printf("[ GRANDPARENT ] Vetor final: ");
        for (int i = 0; i < tamvec; i++)
        {
            printf("%d ", vec[i]);
        }
    }
    else
    {
        if (read(fdPipe[0],vec,tamvec*sizeof(int))==-1)
        {
            perror("Erro na leitura do processo PARENT");
            _exit(1);
        }
        for (int i = 0; i < tamvec; i++)
        {
            vec[i]*=3;
        }

        pid2=fork();
        if (pid2<0)
        {
            perror("Erro no fork");
            _exit(1);
        }

        if (pid2>0)
        {
            if (write(fdPipe[1],vec,tamvec*sizeof(int))==-1)
            {
                perror("Erro na escrita do processo PARENT");
                _exit(1);
            }
            close(fdPipe[1]);
            close(fdPipe[0]);
            if (waitpid(pid2, NULL, 0)==-1)
            {
                perror("Erro na espera do processo CHILD");
                _exit(1);
            }
        }
        

        else
        {
            if (read(fdPipe[0],vec,tamvec*sizeof(int))==-1)
            {
                perror("Erro na leitura do processo CHILD");
                _exit(1);
            }
            close(fdPipe[0]);
            int i=0;
            int j=tamvec-1;
            int aux;
            while (i<j)
            {
                aux=vec[i];
                vec[i]=vec[j];
                vec[j]=aux;
                i++;
                j--;
            }
            if (write(fdPipe[1],vec,tamvec*sizeof(int))==-1)
            {
                perror("Erro na escrita do processo CHILD");
                _exit(1);
            }
            close(fdPipe[1]);
        }
    }
    




    return 0;
}
