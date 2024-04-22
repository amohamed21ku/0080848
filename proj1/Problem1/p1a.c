#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc, char** argv) {
  
  //getting n
  int n = atoi(argv[1]);

  // storing level ,parent and child process id
  int level = 0;
  int child_id = getpid();
  int parent_id;
 
  printf("Main Process ID: %d, level: %d\n", child_id, level);

  for (int i = 0; i < n; i++){
    //create a child process
    child_id = fork();
    //child process
    if (child_id == 0){
      child_id = getpid();
      parent_id = getppid();
      //inside of the child so level is up
      level++;
      printf("Process ID: %d, Parent ID: %d, level: %d\n", child_id, parent_id, level);
    }
    //parent 
    else if (child_id > 0){
      //then wait for a child to change its state
      wait(NULL);
    }
    //pid < 0: error
    else{
      printf("Fork failed");
      return 1;
    }
  }

  return 0;
}