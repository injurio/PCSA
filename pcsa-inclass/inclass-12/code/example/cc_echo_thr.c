#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include "pcsa_net.h"
#include "echo_logic.h"

typedef struct sockaddr SA;

struct survival_bag {
    struct sockaddr_storage clientAddr;
    int connFd;
};

void* conn_handler(void *args) {
    struct survival_bag *context = (struct survival_bag *) args;
    
    pthread_detach(pthread_self());
    echo_logic(context->connFd);
    close(context->connFd);
    
    free(context); /* Done, get rid of our survival bag */
    return NULL; /* Nothing meaningful to return */
}

int main(int argc, char* argv[]) {

    printf("Process-based Concurrent Server\n");

    int listenFd = open_listenfd(argv[1]);


    for (;;) {
        struct sockaddr_storage clientAddr;
        socklen_t clientLen = sizeof(struct sockaddr_storage);
        pthread_t threadInfo;

        int connFd = accept(listenFd, (SA *) &clientAddr, &clientLen);
        if (connFd < 0) { fprintf(stderr, "Failed to accept\n"); continue; }

        struct survival_bag *context = 
            (struct survival_bag *) malloc(sizeof(struct survival_bag));

        context->connFd = connFd;
        memcpy(&context->clientAddr, &clientAddr, sizeof(struct sockaddr_storage));

        pthread_create(&threadInfo, NULL, conn_handler, (void *) context);
    }

    return 0;
}
