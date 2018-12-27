#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define DEBUG 0

void exProcess(char *args[]) {
    if(DEBUG) printf("Child: running execvp()\n");
    int status = execvp(args[0], args);
    
    fprintf(stderr, "Error: execvp()\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if(DEBUG) printf("Initializing Variables\n");

    if(DEBUG) printf("Assigning %d to ARRAY_SIZE\n", argc);

    const int ARRAY_SIZE = argc;
    char *args[ARRAY_SIZE];

    if(DEBUG) printf("Assigning Data to Array\n");

    for(int i = 0; i < (ARRAY_SIZE - 1); i++) {
        int num = i + 1;
        if(DEBUG) printf("Assigning %s to args[%d]\n", argv[num], i);

        args[i] = argv[num];
    }

    if(DEBUG) printf("Null Terminating Array\n");

    args[ARRAY_SIZE - 1] = NULL;

    if(DEBUG) {
        for(int i = 0; i < ARRAY_SIZE; i++)
            printf("args[%d]: %s\t", i, args[i]);

        printf("\n");
    }

    if(DEBUG) printf("Init fork()");

    pid_t pid = fork();

    if(pid < 0) {
        fprintf(stderr, "Error: fork()\n");

        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if(DEBUG) printf("Inside Child Process\n");

        exProcess(args);

        exit(EXIT_FAILURE);
    }

    if(DEBUG) printf("Parent: Analysing Child Process\n");

    int wWstatus;
    int wPStatus;
    pid_t wPPid = waitpid(pid, &wPStatus, WNOHANG);
    if(wPPid < 0) {
        fprintf(stderr, "Error: waitpid()\n");

        exit(EXIT_FAILURE);
    } else if(wPPid == 0) {
        if(DEBUG) printf("Parent: Waiting on Child Process to Finish\n");

        int wStatus;
        pid_t wPid = wait(&wStatus);

        wWstatus = WEXITSTATUS(wStatus);
    } else {
        if(DEBUG) printf("Parent: Child Process Already Finished, Wrapping up\n");

        wWstatus = WEXITSTATUS(wPStatus);
    }

    printf("Program: \"%s\" exited with code: %d\n", argv[1], wWstatus);

    if (wWstatus != 0)
        printf("This program more than likely exited with and error\n");
    else
        printf("This Program more than likely exited without any errors\n");

    return 0;
}
