// Yan Castello Simmer ; Matrícula 2026100407
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
// For the syscall functions.
// For wait and related macros.
int main(int argc, char *argv[]) {
   pid_t pid = fork(); // Fork a child process.
   char *args[3];

    if (argc > 2) {
        args[0] = argv[1];
        args[1] = argv[2]; 
        args[2] = NULL;    
    } else {
        args[0] = argv[1]; 
        args[1] = NULL;    
    }
   
   if (pid < 0) { // Error occurred.
      fprintf(stderr, "Fork failed!\n");
      return 1;
   } else if (pid == 0) { // Child process.
      printf("[CHILD]: About to load command.\n");
      execvp(args[0], args );  
   } else { // Parent process.
      printf("[PARENT]: Waiting on child.\n");
      wait(NULL);
      printf("[PARENT]: Child finished.\n");
   }
   return 0;
}
