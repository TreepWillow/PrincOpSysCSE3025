#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char *argv[]) {

    
    if (argc > 2){
        printf("Error: Too many arguments. (txtmsg <message>)\n");
        exit(0);
    }
    
    char buffer[100];

    // Make a pipe to talk over
    int fd[2];
    
    if (pipe(fd) == -1){
        printf("Error: pipe failed\n");
        exit(0);
    }


    // Make child proccess
    pde_t pid = fork();

    if (pid < 0){
        printf("Error: fork failed\n");
        exit(0);
    }

    if (pid == 0){ // the child will see its returned pid as 0
        //printf("Ping");

        read(fd[0],buffer,sizeof(buffer));
        printf("%d: recieved %s\n",getpid(),buffer);
        write(fd[1],buffer,sizeof(buffer));
        close(fd[0]);
        close(fd[1]);
        return 42;
        

    } else { // if pid is not 0 then it is the parent
        printf("Parent pid: %d\nChild pid: %ld\n", getpid(), pid);
    
        int status;
        // Send message
        write(fd[1],argv[1],strlen(argv[1]) + 1);

        wait(&status);

        // Read message and print
        read(fd[0],buffer,sizeof(buffer));
        printf("%d: recieved %s\n",getpid(),buffer);
        close(fd[0]);
        close(fd[1]);
    }

    exit(0);
}