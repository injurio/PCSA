#include<sys/types.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include "pcsa_net.h"
#include "echo_logic.h"

typedef struct sockaddr SA;

void sigchld_handler(int sig) {
    /* Reap all zombie children */
    while (waitpid(-1, 0, WNOHANG) > 0)
        continue;
}

int main(int argc, char* argv[]) {

    printf("Process-based Concurrent Server\n");

    int listenFd = open_listenfd(argv[1]);

    signal(SIGCHLD, sigchld_handler);

    for (;;) {
        struct sockaddr_storage clientAddr;
        socklen_t clientLen = sizeof(struct sockaddr_storage);

        int connFd = accept(listenFd, (SA *) &clientAddr, &clientLen);
        if (connFd < 0) { fprintf(stderr, "Failed to accept\n"); continue; }
        
        if (fork() == 0) {
            close(listenFd); /* Child doesn't need listenFd */
            display_connetion(&clientAddr); /* Show connection info */
            echo_logic(connFd); /* Service it */
            close(connFd); /* Child closes the connection */
            exit(0); /* Child is terminating */
        }

        /* IMPORTANTLY: Parent closes connected socket */
        close(connFd);
    }

    return 0;
}
