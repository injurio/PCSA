/* ICCS227: Project 1: icsh
 * Name: Chayathorn Teratanitnan
 * StudentID: 6280945
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>

#define MAX_CMD_BUFFER 255

int i, status, pid;
char *res, last_command[MAX_CMD_BUFFER], *argv[3];
void command(char *), my_history(char *), my_read(char *);

void handler(int signum) {
    if (signum == SIGINT && pid == 0) {
      kill(pid, SIGINT);
    } else if (signum == SIGTSTP && pid == 0) {
      signal(SIGTSTP, SIG_DFL);
    }
}

int main(int argc, char * argv []) {
	
	char buffer[MAX_CMD_BUFFER];
	struct sigaction new_action;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_handler = handler;
  new_action.sa_flags = 0;
  sigaction(SIGINT, &new_action, NULL);
  sigaction(SIGTSTP, &new_action, NULL);
  if (argc > 1) {
		my_read(argv[1]);
	}
	while(1) {
		printf("icsh $ ");
    fgets(buffer, 255, stdin);
    command(buffer);
	}
}

void command(char *buffer) {
    if (strstr(buffer, "echo")){
		my_history(buffer);
        for (res = buffer ; *res && *res != ' ' ; res++)
            ;
        if (*res) res++;
        printf("%s", res);
    } 
    else if (strstr(buffer, "!!")){
		strcpy(buffer, last_command);
		// printf("%s", buffer);
		command(buffer);
    } 
    else if (strstr(buffer, "exit")) {
        for (res = buffer ; *res && *res != ' ' ; res++)
            ;
        if (*res) res++;
        if (atoi(res) && atoi(res) >= 1) {
            // printf("bye\n");
            exit(atoi(res));
        } 
		// else { printf("bad command\n"); }
    } else if (strstr(buffer, ">")) {
      char * token = strtok(buffer, ">");
      char *p = strrchr(buffer, ' ');
      int file_desc = open(p+2, O_TRUNC | O_CREAT | O_WRONLY, 0666);
      if ((pid=fork())==0){
        dup2(file_desc, 1);
        system(token);
        close(file_desc);
      }
      close(file_desc);
    }
      else {
		if ((pid = fork()) == 0) {
			system(buffer);
      exit(0);
		} else if (pid < 0) {
      perror("Create Fork Error");
    }
		waitpid(pid, NULL, 0);
	}
}

void my_history(char *buffer){
	strcpy(last_command, buffer);
}

void my_read(char *filename) {
	FILE* ptr;
    ptr = fopen(filename, "r");
	char line[500];
	while (fgets(line, sizeof(line), ptr)) {
		command(line);
	}
    fclose(ptr);
}
