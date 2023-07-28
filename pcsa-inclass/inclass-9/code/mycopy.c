#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

#define BUFSIZE 512

int main(int argc, char* argv[]) {
    int inFd, outFd;
    char buf[BUFSIZE];

    inFd = open(argv[1], O_RDONLY);
    outFd = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0644);

    ssize_t numRead;

    clock_t begin = clock();
    while ((numRead = read(inFd, buf, BUFSIZE)) > 0) {
        write(outFd, buf, numRead);
    }
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("done: %lf\n", time_spent);

    close(inFd);
    close(outFd);

    return 0;
}
