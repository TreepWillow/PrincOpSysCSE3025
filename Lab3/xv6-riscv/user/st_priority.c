// user/st_priority.c
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pid = atoi(argv[1]);
    int prio = atoi(argv[2]);
    if(set_priority(pid, prio) < 0)
        printf("Failed to set priority for pid %d\n", pid);
    else
        printf("Priority of pid %d set to %d\n", pid, prio);
    exit(0);
}
