/* Oráculo SEQUENCIAL de entrada/saída para validar os casos.
 * NÃO é solução dos exercícios: não cumpre suas exigências concorrentes.
 * O caso p02 usa um pipeline real para verificar a injeção de ping via PATH.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#ifndef EX
#define EX ""
#endif
int main(void) {
    long long a[1000], total=0;
    int n=10,t=2;
    if (!strcmp(EX,"p02")) {
        int fd[2]; if(pipe(fd)) return 2;
        pid_t p=fork(); if(p<0)return 2;
        if(!p) {close(fd[0]);dup2(fd[1],1);close(fd[1]);execlp("ping","ping","-c","5","google.com",(char*)NULL);_exit(127);}
        pid_t q=fork(); if(q<0)return 2;
        if(!q) {close(fd[1]);dup2(fd[0],0);close(fd[0]);execlp("grep","grep","rtt",(char*)NULL);_exit(127);}
        close(fd[0]);close(fd[1]); int sp,sq;
        waitpid(p,&sp,0);waitpid(q,&sq,0);
        return WIFEXITED(sp)&&WEXITSTATUS(sp)==0&&WIFEXITED(sq)&&WEXITSTATUS(sq)<=1?0:2;
    }
    if(!strcmp(EX,"s16")) { int k;if(scanf("%d%d",&t,&k)!=2)return 2;printf("%lld\n",(long long)t*k);return 0; }
    if(!strcmp(EX,"p01")) {long long x;if(scanf("%lld",&x)!=1)return 2;printf("[PARENT] Resultado Final: %lld!\n",x+15);return 0;}
    if(!strcmp(EX,"t06")||!strcmp(EX,"t10")) {
        const int primes[]={2,3,5,7,11,13,17,19,23,29};
        for(int i=0;i<10;i++) {total+=primes[i];if(!strcmp(EX,"t10"))printf("%d %d\n",i,primes[i]);}
        if(!strcmp(EX,"t06"))printf("%lld\n",total);
        return 0;
    }
    if(!strcmp(EX,"x03")) {n=1000;if(scanf("%d",&t)!=1)return 2;if(t<1||t>1000)return 1;}
    else if(!strcmp(EX,"x04")) {if(scanf("%d%d",&n,&t)!=2)return 2;if(n<0||n>1000||t<1||t>32)return 1;}
    else if(!strcmp(EX,"p03")||!strcmp(EX,"p09")) {if(scanf("%d",&n)!=1)return 2;}
    else if(!strcmp(EX,"m07")) {if(scanf("%d%d",&t,&n)!=2)return 2;}
    else if(!strcmp(EX,"t12")) n=4;
    for(int i=0;i<n;i++){if(scanf("%lld",&a[i])!=1)return 2;total+=a[i];}
    if(!strcmp(EX,"t07")) {for(int i=n-1;i>=0;i--)printf("%d %lld\n",i,a[i]);}
    else if(!strcmp(EX,"t12")) {for(int i=0;i<n;i++)printf("%lld ",a[i]*a[i]);puts("");}
    else if(!strcmp(EX,"p03")) {printf("[GRANDPARENT] Vetor final:");for(int i=n-1;i>=0;i--)printf(" %lld",3*a[i]);puts("");}
    else printf("%lld\n",total);
    return 0;
}
