#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "pcsa_net.c"

#define BUFSIZE 1024
typedef struct sockaddr SA;

void respond_with_file(int connFd, const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (file == NULL) {
        // File not found, send 404 response
        char *msg = "404 Not Found";
        char buf[BUFSIZE];
        sprintf(buf, "HTTP/1.1 404 Not Found\r\n"
        "Server: Tiny\r\n"
        "Content-length: %lu\r\n"
        "Connection: close\r\n"
        "Content-type: text/plain\r\n\r\n", strlen(msg));
        write_all(connFd, buf, strlen(buf));
        write_all(connFd, msg, strlen(msg));
        return;
    }

    // Get file size
    struct stat st;
    stat(filepath, &st);
    off_t file_size = st.st_size;

    // Determine MIME type based on file extension
    const char* mime_type = "text/plain";
    const char* extension = strrchr(filepath, '.');
    if (extension != NULL) {
        if (strcmp(extension, ".html") == 0)
            mime_type = "text/html";
        else if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0)
            mime_type = "image/jpeg";
    }

    // Send HTTP response header
    char buf[BUFSIZE];
    sprintf(buf, "HTTP/1.1 200 OK\r\n"
    "Server: Tiny\r\n"
    "Content-length: %lu\r\n"
    "Connection: close\r\n"
    "Content-type: %s\r\n\r\n", file_size, mime_type);
    write_all(connFd, buf, strlen(buf));

    // Send file contents
    char file_buf[BUFSIZE];
    size_t bytes_read;
    while ((bytes_read = fread(file_buf, sizeof(char), BUFSIZE, file)) > 0) {
        write_all(connFd, file_buf, bytes_read);
    }

    fclose(file);
}

void serve_http(int connFd, const char* rootFolder) {
    char buf[BUFSIZE];

    if (!read_line(connFd, buf, BUFSIZE))
        return;

    printf("LOG: %s\n", buf);

    char method[BUFSIZE], uri[BUFSIZE], version[BUFSIZE];
    sscanf(buf, "%s %s %s", method, uri, version);

    while (read_line(connFd, buf, BUFSIZE) > 0) {
        if (strcmp(buf, "\r\n") == 0)
            break;
    }

    if (strcmp(method, "GET") == 0) {
        // Construct file path based on root folder and requested URI
        char filepath[BUFSIZE];
        if (!strcmp(uri, "/")) {
            snprintf(filepath, BUFSIZE, "%s/index.html", rootFolder);
        } else {
            snprintf(filepath, BUFSIZE, "%s%s", rootFolder, uri);
        }

        if (access(filepath, F_OK) == 0) {
            respond_with_file(connFd, filepath);
            return;
        } else {
            perror("Cannot access file");
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <portNum> <rootFolder>\n", argv[0]);
        return 1;
    }

    // Open listening socket
    int listenFd = open_listenfd(argv[1]);

    for (;;) {
        struct sockaddr_storage clientAddr; // to store addr of the client
        socklen_t clientLen = sizeof(struct sockaddr_storage); // size of the above

        // ...gonna block until someone connects to our socket
        int connFd = accept(listenFd, (SA*)&clientAddr, &clientLen);

        // print the address of the incoming client
        char hostBuf[BUFSIZE], svcBuf[BUFSIZE];
        if (getnameinfo((SA*)&clientAddr, clientLen, hostBuf, BUFSIZE, svcBuf, BUFSIZE, 0) == 0)
            printf("Connection from %s:%s\n", hostBuf, svcBuf);
        else
            printf("Connection from UNKNOWN.\n");

        serve_http(connFd, argv[2]); // service the client on this fd
        close(connFd);
    }

    return 0;

}