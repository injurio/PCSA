#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include "pcsa_net.h"

#define BUFSIZE 255

typedef struct sockaddr SA;

void echo_logic(int connFd) {
    ssize_t bytesRead;
    char buf[BUFSIZE];

    while ((bytesRead = read(connFd, buf, BUFSIZE)) > 0) {
        printf("DEBUG: Read %ld bytes\n", bytesRead);
        write_all(connFd, buf, bytesRead);
    }
    printf("DEBUG: Connection closed\n");
}

void display_connetion(struct sockaddr_storage *clientAddr) {
    socklen_t clientLen = sizeof(struct sockaddr_storage);
    char hostBuf[BUFSIZE], svcBuf[BUFSIZE];
    if (getnameinfo((SA *) clientAddr, clientLen, 
                    hostBuf, BUFSIZE, svcBuf, BUFSIZE, 0)==0) 
        printf("Connection from %s:%s\n", hostBuf, svcBuf);
    else
        printf("Connection from ?UNKNOWN?\n");
}

