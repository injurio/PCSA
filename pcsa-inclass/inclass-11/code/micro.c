#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include "pcsa_net.c"

#define BUFSIZE 1024
typedef struct sockaddr SA;

void respond_with_file(int connFd, const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if (file == NULL) {
        // file not found, send 404 response
        char *msg = "<h1>404 Not Found</h1>";
        char buf[BUFSIZE];
        sprintf(
            buf, 
            "HTTP/1.1 404 Not Found\r\n"
            "Server: Micro\r\n"
            "Content-length: %lu\r\n"
            "Connection: close\r\n"
            "Content-type: text/html\r\n\r\n", 
            strlen(msg)
        );
        write_all(connFd, buf, strlen(buf));
        write_all(connFd, msg, strlen(msg));
        return;
    }

    // get file size
    struct stat st;
    stat(filepath, &st);
    off_t file_size = st.st_size;

    // determine MIME type based on file extension
    const char* mime_type = "text/plain";
    const char* extension = strrchr(filepath, '.');
    if (extension != NULL) {
        if (strcmp(extension, ".html") == 0) {
            mime_type = "text/html";
        } else if (strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
            mime_type = "image/jpeg";
        }
    }

    // send HTTP response header
    char buf[BUFSIZE];
    sprintf(
        buf, 
        "HTTP/1.1 200 OK\r\n"
        "Server: Micro\r\n"
        "Content-length: %lu\r\n"
        "Connection: close\r\n"
        "Content-type: %s\r\n\r\n", 
        file_size, 
        mime_type
    );
    write_all(connFd, buf, strlen(buf));

    // send file contents
    char file_buf[BUFSIZE];
    size_t bytes_read;
    while ((bytes_read = fread(file_buf, sizeof(char), BUFSIZE, file)) > 0) {
        write_all(connFd, file_buf, bytes_read);
    }

    fclose(file);
}

void serve_http(int connFd, const char* rootFolder) {
    char buf[BUFSIZE];

    if (!read_line(connFd, buf, BUFSIZE)) {
        return;
    }

    printf("LOG: %s\n", buf);

    char method[BUFSIZE], uri[BUFSIZE], version[BUFSIZE];
    sscanf(buf, "%s %s %s", method, uri, version);

    while (read_line(connFd, buf, BUFSIZE) > 0) {
        if (strcmp(buf, "\r\n") == 0) {
            break;
        }
    }

    if (strcmp(method, "GET") == 0) {
        // construct file path based on root folder and requested URI
        char filepath[BUFSIZE];
        if (!strcmp(uri, "/")) {
            snprintf(filepath, BUFSIZE, "%s/index.html", rootFolder);
        } else {
            snprintf(filepath, BUFSIZE, "%s%s", rootFolder, uri);
        }
        respond_with_file(connFd, filepath);
    }
}

// ./micro <portNum> <rootFolder>
int main(int argc, char* argv[]) { 
    // fd for listening for incoming connn
    int listenFd = open_listenfd(argv[1]);

    // root folder
    char* rootFolder = argv[2];

    while (1) {
        // store addr of the client
        struct sockaddr_storage clientAddr; 
        // size of the above
        socklen_t clientLen = sizeof(struct sockaddr_storage); 

        // block until connects to the socket
        int connFd = accept(listenFd, (SA *) &clientAddr, &clientLen);

        // print the address of the incoming client
        char hostBuf[BUFSIZE], svcBuf[BUFSIZE];
        if (getnameinfo((SA *) &clientAddr, clientLen, hostBuf, BUFSIZE, svcBuf, BUFSIZE, 0) == 0) {
            printf("Connection from %s:%s\n", hostBuf, svcBuf); 
        } else {
            printf("Connection from UNKNOWN.");
        }

        serve_http(connFd, rootFolder);

        close(connFd);
    }

    return 0;

}

