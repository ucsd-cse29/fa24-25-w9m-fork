#include <stdio.h>
#include <unistd.h>


int main() {
  // pid stands for "process id"
  pid_t result = fork();
  if(result) {
    while(1) {
      printf("fork returned true %d\n", result);
    }
  }
  else {
    while(1) {
      printf("fork returned false (0)\n");
    }
  } 
} 
