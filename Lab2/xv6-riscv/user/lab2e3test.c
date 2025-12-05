#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]){
    
    
    
    // fork process
    pde_t pid = fork();


    if (pid < 0){
        printf("Error: fork failed\n");
        exit(0);
    }

    if (pid == 0){ // the child will see its returned pid as 0
        
        printf("child_pid: %d nice: %d",getpid(),get_priority(getpid()));

        set_priority(getpid(),5);
        printf("child_pid: %d new nice: %d",getpid(),get_priority(getpid()));
        
        return 42;
        

    } else { // if pid is not 0 then it is the parent
        
    
        int status;
       
        wait(&status);
        printf("Parent pid: %d\nChild pid: %ld\n", getpid(), pid);
        
    }

    return 0;
}