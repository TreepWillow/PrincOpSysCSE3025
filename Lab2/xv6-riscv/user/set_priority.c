#include "user/user.h"

int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Usage: set_priority <pid> <nice_value>\n");
        exit(1);
    }

    // char to num
    int pid = atoi(argv[1]);
    int nice = atoi(argv[2]);

    // auto clamp values to limits
    if (nice < 0){nice = 0;}
    if (nice > 39){nice = 39;}

    // call the syscall of set priority
    if (set_priority(pid,nice) < 0) {
        printf("Failed to set priority\n");
        exit(1);
    }
    
    return 0;
}