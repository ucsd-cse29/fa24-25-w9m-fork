#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int main() {
  char command[256];
  while(1) {
    printf(" ~> ");
    fgets(command, sizeof(command), stdin);
    command[strlen(command) - 1] = 0;

    pid_t result = fork();
    // in the parent proces.... ???
    if(result == 0) {
      char* args[] = { command, NULL };
      execvp(command, args);
      printf("Could not run the command: %s\n", command);
      exit(29);
    }
    else {
      printf("PID: %d\n", result);
      int status;
      // wait: returns when a child process finishes
      wait(&status);
      printf("The child process finished: %d\n", WEXITSTATUS(status));
    }
/* We saw:

    [cs29fa24@ieng6-201]:proc:516$ ./shell                 
 ~> ls                                                 
 ~> exec  exec.c  out.txt  proc  proc.c  shell  shell.c

  The printing of ~> (the prompt) is RACING with the
  command/execvp process.

  We would like to guarantee that the command output
  happens BEFORE we continue the loop
*/

  }

}
