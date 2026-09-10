#include <stdio.h>
#include <unistd.h>

int main ()
{

    int getPid = 0;

    for (int i = 0; i < 4; i++)
    {
        if (getPid==0)
        {
            getPid=fork();
        }
        
    }
    
    printf("Im a process\n");

    return 0;
}