#include <stdlib.h>
#include <stdio.h>
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <wait.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <string.h>
#include <ifaddrs.h>
#include <malloc.h>

#define DEBUG 1

void installProcess(char *args[]) {
    int status = execvp("apt-get", args);
    if(status < 0) {
        fprintf(stderr, "Error: execvp()");
        exit(EXIT_FAILURE);
    }
}

void dpkgProcess(char *args[]) {
    int status = execvp("dpkg", args);
    if(status < 0) {
        fprintf(stderr, "Error: execvp()");
        exit(EXIT_FAILURE);
    }
}

int checkPSCP(int code) {
    if(code != 0)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}

void exProcess(char *args[]) {
    int status = execvp("pscp", args);
    if(status < 0) {
        fprintf(stderr, "Error: execvp()");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    if(getuid() != 0) {
        fprintf(stderr, "Please use ROOT\n");
        exit(EXIT_FAILURE);
    }

    if(argc != 3) {
        fprintf(stderr, "%s <DIRECTORY> <FILENAME>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(DEBUG) printf("Writing strings\n");
    char *file = malloc(sizeof(char) * 256);
    char *command = malloc(sizeof(char) * 256);
    strcpy(command, "okole@10.17.45.226:");
    strncat(command, argv[1], (strlen(command) + strlen(argv[1])));
    strcpy(file, argv[2]); 

    char *args[] = { "pscp", file, command, NULL };

    if(DEBUG) printf("Creating ip vars\n");
    const char *server = "10.17.45.226";

    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }


    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL)
            continue;  

        s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if((strcmp(ifa->ifa_name, "wlp4s0b1") == 0 || strcmp(ifa->ifa_name, "wlp1s0") == 0) 
            && (ifa->ifa_addr->sa_family == AF_INET)) {
            if (s != 0) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }

            printf("\tInterface : <%s>\n",ifa->ifa_name );
            printf("\t  Address : <%s>\n", host); 
        }
    }

    if(DEBUG) {
        printf("host: %s\nserver: %s\n", host, server); 
        printf("Forking... \n");
    }
    pid_t pid = fork();
    if(pid < 0) {
        fprintf(stderr, "Error: fork()");
    }

    sleep(2);

    printf("\n");

    if(pid == 0) {

        if(DEBUG) printf("Forking a process to check dependicies\n");

        pid_t cPid = fork();
        if(cPid < 0) {
            fprintf(stderr, "Error: fork()\n");
            exit(EXIT_FAILURE);
        } else if(cPid == 0) {
            char buf[20];
            int saved_stdout = dup(1);

            freopen("/dev/null", "w", stdout);

            char *args[] = {"dpkg", "-s", "putty-tools", NULL};
            dpkgProcess(args);

            // sprintf(buf, "/dev/fd/%d", saved_stdout);
            // freopen(buf, "w", stdout);

            exit(EXIT_SUCCESS);
        }

        int dWEStatus = 0;
        int wDStatus = 0;
        int dWPStatus;
        pid_t dWPPid = waitpid(cPid, &dWPStatus, WNOHANG);
        if(dWPPid < 0) {
            fprintf(stderr, "Error: waitpid()\n");
            exit(EXIT_FAILURE);
        } else if(dWPPid == 0) {
            int dStatus;
            pid_t dPid = wait(&dStatus);
            wDStatus = WEXITSTATUS(dStatus);

            if(DEBUG) printf("dPid Status: %d\n", wDStatus);
        } else {
            dWEStatus = WEXITSTATUS(dWPStatus);
            if(DEBUG) printf("dPid Status: %d\n", dWEStatus);
        }

        if(dWEStatus != 0 || wDStatus != 0) {
            printf("Requirements not installed... fixing...\n");
            pid_t iPid = fork();
            if(iPid < 0) {
                fprintf(stderr, "Error: fork()");
                exit(EXIT_FAILURE);
            } else if(iPid == 0) {
                char *args[] = {"apt-get", "install", "putty-tools", NULL};
                installProcess(args);

                exit(EXIT_SUCCESS);
            }

            int iWWStatus;
            int iWPStatus;
            pid_t iWPPid = waitpid(iPid, &iWPStatus, WNOHANG);
            if(iWPPid < 0) {
                fprintf(stderr, "Error: waitpid()\n");
                exit(EXIT_FAILURE);
            } else if(iWPPid == 0) {
                int iWStatus;
                pid_t iWPid = wait(&iWStatus);
                iWWStatus = WEXITSTATUS(iWStatus);
            } else {
                iWWStatus = WEXITSTATUS(iWPStatus);
            }

            printf("iPid exit status: %d", iWWStatus);

            if(iWWStatus != 0) {
                fprintf(stderr, "Error: apt-get failed...\n");
                exit(EXIT_FAILURE);
            }
        }

        if(strcmp(host, server) != 0)
            exProcess(args);
        else {
            printf("You're already on the file server... Exiting...");
            exit(EXIT_SUCCESS);
        }
    }

    int status;
    pid_t wPPid = waitpid(pid, &status, WNOHANG);
    if(wPPid < 0) {
        fprintf(stderr, "Error: waitpid()\n");
        exit(EXIT_FAILURE);
    } else if(wPPid == 0) {
        int wStatus;
        pid_t wPid = wait(&wStatus);
        int wWStatus = WEXITSTATUS(wStatus);

        if(checkPSCP(wStatus) == EXIT_FAILURE) {
            fprintf(stderr, "Error: PSCP failed with code: %d\n", wWStatus);
        } else {
            printf("PSCP Finished with status %d\n", wWStatus);
        }
    } else {
        int wWPStatus = WEXITSTATUS(status);
        if(checkPSCP(wWPStatus) == EXIT_FAILURE) {
            fprintf(stderr, "Error: PSCP failed with code: %d\n", wWPStatus);
        } else {
            printf("PSCP Finished with status %d\n", wWPStatus);
        }
    }

    free(file);
    free(command);

    freeifaddrs(ifaddr);
    return 0;
}
