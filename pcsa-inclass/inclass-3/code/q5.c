#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  for (int i=0;i<5;i++)
    if (fork() == 0) {
      sleep(1); // Pause the process for 1 second
      printf("%d\n", i);
      return 0; // Return here
    }
  int ret;
  wait(&ret); // Wait here
  // Added
  // printf("ret: %d\n", ret);
  printf("Done\n");
  return 0;
}