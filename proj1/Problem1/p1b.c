#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
  int pid;
  char *command_name = argv[1];
	
  pid = fork();

	if(pid == 0) {
    //zombie
		exit(0);
// means it is a parent
	} else if (pid > 0) {
		//sleep for 6s
    sleep(6); 
    wait(NULL);
    

	} else {
    printf("Fork failed");
    return 1;
	}
	return 0;
}