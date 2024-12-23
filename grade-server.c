#include "http-server.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

char const* HTTP_200_OK = "HTTP/1.1 200 OK\nContent-Type: text/plain; charset=UTF-8\n\n";
char const* HTTP_404_NOT_FOUND = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found\n";

uint8_t hex_to_byte(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    return 0;
}

void url_decode(char *src, char *dest) {
    // iterate through the src string until you hit a \0
    char *p_src = src;
    char *p_dest = dest;

    while (*p_src) { // for (int i = 0; p_src[i] != 0; i++)
        if (*p_src == '%') {

            // ex: %20, %25 etc
            uint8_t f = hex_to_byte(*(p_src + 1));
            uint8_t s = hex_to_byte(*(p_src + 2));

            uint8_t final = (f*16) + s;
            *p_dest = (unsigned char) final;

            // increment p_src
            p_src += 2;


        } else {
            // not percent-encoded
            *p_dest = *p_src;

        }
        p_src++;
        p_dest++;
    }
    *p_dest = '\0';
}



void handle_response(char *request, int client_sock) {
    if(strlen(request) >= BUFFER_SIZE -1) {
        printf("Request too long\n");
        write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
        return;
    }

    char command[] = "./grade.sh";
    char reponame[32];
    char decoded[BUFFER_SIZE];

    char* question = strstr(request, "?");
    if(question == NULL) {
      write(client_sock, HTTP_404_NOT_FOUND, strlen(HTTP_404_NOT_FOUND));
      return;
    }

    char* lastspace = strstr(question, " ");
    *lastspace = 0;
    url_decode(question + 1, reponame);

    printf("%p %p\n", reponame, command);
    printf("%s %s\n", reponame, command);
    printf("The reponame is %s\n", reponame);

    pid_t result = fork();
    if(result != 0) {
      int status;
      wait(&status);
      write(client_sock, reponame, strlen(reponame));
    }
    else {
      write(client_sock, HTTP_200_OK, strlen(HTTP_200_OK));
      char* args[] = { command, reponame, NULL };
      dup2(client_sock, STDOUT_FILENO);
      dup2(client_sock, STDERR_FILENO);
      execvp(command, args);
      printf("execvp failed for %s\n", command);
      exit(1);
    }
}


int main(int argc, char *argv[]) {
    int port = 0;
    if(argc >= 2) {
        port = atoi(argv[1]);
    }
    start_server(&handle_response, port);
}
