#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"


#define NCHILD 5

int
main(void)
{
  int i, pid;

  for (i = 0; i < NCHILD; i++) {
    pid = fork();
    if (pid < 0) {
      printf("Fork failed\n");
      exit(0);
    }
    if (pid == 0) {
      // Child process
      int prio = 11 + i * 5;   // priorities: 5, 10, 15, 20, 25
      //printf("%d\n", prio);
      set_priority(getpid(), prio);
      //int actual = get_priority(getpid());
      //printf("Child %d (pid=%d) set to priority %d\n", i, getpid(), actual);

      // Simple workload
      for (volatile int j = 0; j < 1000000000; j++);

      //printf("Child %d (pid=%d) finished\n", i, getpid());
      exit(0);
    }
    // Parent continues loop
  }

  // Parent waits for all children
  for (i = 0; i < NCHILD; i++) {
    wait(0);
  }

  //printf("All children finished\n");
  exit(0);
}
