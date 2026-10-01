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
    pipe(fdPipe1);
    pid1 = fork();
    if (pid1>0)

    {   pipe(fdPipe3);
        pid2 = fork();
        if (pid2>0)
        {
            close(fdPipe1[0]);
            close(fdPipe3[1]);
            scanf("%d",&num);
            write(fdPipe1[1],num,4);
        }
        
    }

    else
    {
        pipe(fdPipe2);
        close(fdPipe2[0]);
        close(fdPipe1[1]);
        read(fdPipe1[0], num, 4);
        num=num+5;
        write(fdPipe2[1],num,4);
    }

    if (pid2==0)
    {
        close(fdPipe1[1]);
        close(fdPipe1[0]);
        close(fdPipe2[1]);
        close(fdPipe3[0]);
    }
    

    return 0;

}