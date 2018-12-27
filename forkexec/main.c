#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int childProcess(char *arguments[]) {
    int eStatus = execvp("ls", arguments);
    if(eStatus == -1) {
        fprintf(stderr, "Error: execvp()\n");
        return EXIT_FAILURE;
    }
}

int parentProcess() {
    int wStatus = 0;
    pid_t wPid = wait(&wStatus);
    int wEStatus = WEXITSTATUS(wStatus);

    if(wPid < 0) {
        fprintf(stderr, "Error: wait()\n");
        return EXIT_FAILURE;
    } else 
        return wEStatus;
}

int main(int argc, char const *argv[])
{
    if(argc < 2) {
        printf("%s <DIRECTORY>", argv[0]);
        exit(2);
    }

    pid_t mPid = fork();
    if(mPid < 0) {
        fprintf(stderr, "Error: fork()\n");
        exit(EXIT_FAILURE);
    }

    char *dir = malloc(sizeof(char) * 256);
    if(dir == NULL) {
        fprintf(stderr, "Error: malloc()");
        exit(EXIT_FAILURE);
    }

    if(strlen(argv[1]) >= 256) {
        fprintf(stderr, "Error: Directory is too big...\n");
        exit(EXIT_FAILURE);
    } else
        strcpy(dir, argv[1]);
    
    char *args[] = {"ls", dir, NULL};

    if(mPid == 0) {
        printf("\nChild: Child Process Initiated...\n");

        int cFStatus = childProcess(args);
        if(cFStatus == EXIT_FAILURE)
            exit(EXIT_FAILURE);

    } else if(mPid > 0) 
        printf("\nParent: Parent Function intact...\n");

    int wPStatus = 0;
    pid_t wPPid = waitpid(mPid, &wPStatus, WNOHANG);
    if(wPPid < 0) {
        fprintf(stderr, "Error: waitpid()\n");
        exit(EXIT_FAILURE);
    } else if(wPPid == 0) {
        printf("\nParent: Waiting on child to finish...\n");

        int pPStatus = parentProcess();
        if(pPStatus == EXIT_FAILURE)
            exit(EXIT_FAILURE);

        printf("Parent: Child program exited with status: %d", pPStatus);
    } else {
        printf("\nParent: Child Process has already finished...\n");

        int wPEStatus = WEXITSTATUS(wPStatus);
        printf("Parent: Child program exited with status: %d\n", wPEStatus);
    }

    printf("\a");

    free(dir);
    return EXIT_SUCCESS;
}
