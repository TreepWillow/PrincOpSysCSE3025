#include "user/user.h"

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("Usage: get_priority <pid>\n");
        exit(1);
    }

    // char to num
    int pid = atoi(argv[1]);


    // call the syscall of get priority
    int retval = get_priority(pid);
    if (retval == -1) {
        printf("Failed to get priority\n");
        exit(1);
    }
    
    printf("pid: %d nice: %d\n", pid, retval);
    return 0;
}