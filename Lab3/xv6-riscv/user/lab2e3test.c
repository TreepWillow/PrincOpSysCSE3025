// Libraries
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // Get process id and print it with it's priority. 
  int pid = getpid();
  printf("Initial priority of pid %d: %d\n", pid, get_priority(pid));

  // Setting the priority to 5. 
  printf("Setting priority to 5...\n");
  if (set_priority(pid, 5) < 0)
    printf("set_priority failed\n");

  // get's priority after change
  int prio = get_priority(pid);
  printf("Priority after change: %d\n", prio);
  
  // Try out-of-range
  set_priority(pid, 100);
  printf("Clamped priority: %d\n", get_priority(pid));
  
  exit(0);
}
