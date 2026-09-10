#include <stdio.h>
#include <unistd.h>     // For the syscall functions.
#include <sys/wait.h>   // For wait and related macros.

int main() {
    pid_t pid = fork(); // Fork a child process.
    if (pid < 0) { // Error occurred.
        fprintf(stderr, "Fork failed!\n");
        return 1;
    } else if (pid == 0) { // Child process.
        printf("[CHILD]: I'm waiting to be signaled. PID: %d\n", getpid());
        for(;;);
    } else { // Parent process.
        printf("[PARENT]: Waiting on child.\n");
        int wstatus;
        waitpid(pid,&wstatus,WUNTRACED);
        if (WIFSIGNALED(wstatus)) {
            printf("[PARENT]: Child SIGNALED with code %d.\n",
                   WTERMSIG(wstatus));
        }
        if (WIFSTOPPED(wstatus)) {
            printf("[PARENT]: Child STOPPED with code %d.\n",
                   WSTOPSIG(wstatus));
        }
    }
    return 0;
}
