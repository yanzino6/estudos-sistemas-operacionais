#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main ()
{
    int fdPipe[2];
    pid_t pid;
    pipe(fdPipe);

    pid = fork();

    if (pid>0)
    {
        dup2(fdPipe[1],1);
        close(fdPipe[0]);
        close(fdPipe[1]);
        execlp("ping", "ping", "-c", "5", "google.com", NULL);
    
    }
    else
    {
        dup2(fdPipe[0],0);
        close(fdPipe[1]);
        close(fdPipe[0]);
        execlp("grep", "grep", "rtt", NULL);
    }
    

}