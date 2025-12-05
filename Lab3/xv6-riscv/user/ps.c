#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 1) {
        printf("Usage: ps\n");
        exit(1);
    }

    int retval = cps();

    //printf("cps returned: %d\n",retval);    

    return retval;
}

