#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(){

    printf("Hello World\n");


    exit(0);
}


/*Reference for myself: What I did to make the custom User command is to add the file include the custom libraries
put in the basic C code + exit(0) and then put the file command name into the UPROGS section
in the Makefile*/