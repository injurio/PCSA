/*
Part I: Buffer Size

How does BUFSIZE affect copy performance? 
Try different buffer sizes and try it on large enough files (like 100MB). 
Vary the buffer size from say 1 byte to a few megabytes. 

Ans: 

Buffer size does have a significant impact on the copy performance.
This is because it dictactes the amount of data read from the input file
and writes to the output file.

From trying different sizes of buffer, it seems that when the buffer size is too small,
the operation takes a lot of time which makes sense as there would be a lot of calls before
the task completed.

On the other hand, when the buffer is too large, it may cause 
inefficiencies as it would require allocating a large space for the buffer.
Also, if the buffer size is too, too large, it can result in segmentation fault.

What value should we use for BUFSIZE? 
(How is a value of 1 byte different from a value of 1GB?)

Ans:

Let's consider the following

Here is some data collected: 
BUFSIZE = 1             ->      15.315  seconds
BUFSIZE = 512           ->      0.030   seconds
BUFSIZE = 1024          ->      0.021   seconds
BUFSIZE = 2048          ->      0.011   seconds
BUFSIZE = 4096          ->      0.008   seconds
BUFSIZE = 8192          ->      0.007   seconds
BUFSIZE = 1048576       ->      0.005   seconds
BUFSIZE = 10485760      ->      segmentation fault

Note that these numbers are obtained by running mycopy.c on a 10 MB file

From this number, it would suggest that the larger the buffer size, the faster the running time.
However, as mentioned above, it may result in inefficiency in terms of space and can result in unexpected error.

Considering the results, I think that the buffer size of 1024, 2048, and 4096 are quite reasonable.
Despite the improved running time, the effect is diminishing. 

However, in general, I think that it would have to depend on the size of the data that we are working with, 
since we have to consider the trade-off between the number of calls and the memory usage. 

*/

// Part II: Line Count

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]) {

    int fd;
    char buf[BUFSIZE];

    fd = open(argv[1], O_RDONLY);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    ssize_t numRead;

    int lineCount = 0;
    int isTextFile = 1;

    while ((numRead = read(fd, buf, BUFSIZE)) > 0) {
        for (ssize_t i = 0; i < numRead; i++) {
            // only care about '\n' because '\r\n' includes '\n' anyways
            if (buf[i] == '\n') {
                lineCount++;
            } 
            // check if contain non-printable
            else if (buf[i] < 0x20 || buf[i] > 0x7e) {
                isTextFile = 0;
                break;
            }
        }
        // check if still is a text file
        if (!isTextFile) {
            printf("Binary file. Bye.\n");
            break;
        }
    }

    close(fd);

    printf("%d\n", lineCount);

    return 0;

}