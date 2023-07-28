#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include "pcsa_net.h"
#include "echo_logic.h"

typedef struct sockaddr SA;
int main(int argc, char* argv[]) {

    int listenFd = open_listenfd(argv[1]);

    for (;;) {
        struct sockaddr_storage clientAddr;
        socklen_t clientLen = sizeof(struct sockaddr_storage);

        int connFd = accept(listenFd, (SA *) &clientAddr, &clientLen);
        if (connFd < 0) { fprintf(stderr, "Failed to accept\n"); continue; }
        
        display_connetion(&clientAddr); /* Show connection info */
        echo_logic(connFd); /* Service connected client */

        close(connFd);
    }

    return 0;
}
