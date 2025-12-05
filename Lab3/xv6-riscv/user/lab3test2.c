#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int k, n, id;
  volatile long x = 0, z;

  if(argc < 2)
    n = 1;        // default value
  else
    n = atoi(argv[1]);  // from command line

  if(n < 0 || n > 20)
    n = 2;

  for(k = 0; k < n; k++){
    id = fork();
    if(id < 0){
      printf("%d failed in fork!\n", getpid());
      exit(1);
    }
    else if(id == 0){ // child
      for(z = 0; z < 8000000000; z++){
        x = x + 1;
      }
      exit(0);
    }
  }

  // parent waits for all children at the end
  for(k = 0; k < n; k++){
    wait(0);
  }

  exit(0);
}
