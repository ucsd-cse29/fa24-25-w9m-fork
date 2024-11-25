#include "http-server.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

char const* HTTP_200_OK = "HTTP/1.1 200 OK\nContent-Type: text/plain; charset=UTF-8\n\n";
char const* HTTP_404_NOT_FOUND = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found\n";

void handle_response(char *request, int client_sock) {
    if(strlen(request) >= BUFFER_SIZE -1) {
        printf("Request too long\n");
        write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
        return;
    }

    char* question = strstr(request, "?");
    if(question == NULL) {
      write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
      return;
    }

    char* lastspace = strstr(question, " ");
    *lastspace = 0;

    char* reponame = question + 1;
    printf("The reponame is %s\n", reponame);
    
    pid_t result = fork();
    if(result != 0) {
      int status;
      wait(&status);
      write(client_sock, reponame, strlen(reponame));
    }
    else {
      write(client_sock, HTTP_200_OK, strlen(HTTP_200_OK));
      char* args[] = { "./grade.sh", reponame, NULL };
      dup2(client_sock, STDOUT_FILENO);
      dup2(client_sock, STDERR_FILENO);
      execvp("./grade.sh", args);
    }
}


int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) {
        port = atoi(argv[1]);
    }
    start_server(&handle_response, port);
}

