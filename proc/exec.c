#include <stdio.h>
#include <unistd.h>


int main() {
  char* args[] = { "ls", NULL };
  execvp("ls", args);
  printf("I just ran ls and it was great!\n"); // does not print!
}
