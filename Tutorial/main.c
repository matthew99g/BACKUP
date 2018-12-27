#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

void mem_exit(char *, char *, char *, int);

int main(int argc, char const *argv[])
{
    // Argument counter for program use
    if(argc < 3 || argc > 3) {
        fprintf(stderr, "%s FILE DATA\n", argv[0]);
        exit(-1);
    }

    // Variable Dec
    int fd;
    int count;
    int red;

    char buf;

    char *file = malloc(sizeof(char) * 256);
    char *data = malloc(sizeof(char) * 256);
    char *fdata = malloc(sizeof(char) * 256);

    // Memory protection
    if(strlen(argv[1]) > 256 || strlen(argv[2]) > 256) {
        fprintf(stderr, "MEMORY OVERFLOW PROTECTION");
        mem_exit(fdata, data, file, fd);
        exit(-1);
    }

    strcpy(file, argv[1]);
    strcpy(data, argv[2]);

    strncat(data, "\n", 1);

    // Creating/opening File
    fd = open(file, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if(fd == -1) {
        fprintf(stderr, "FATAL ERROR - OPENING FILE");
        mem_exit(fdata, data, file, fd);
        exit(-1);
    }

    // Writing data to file
    write(fd, data, strlen(data));
    if(fd == -1) {
        fprintf(stderr, "FATAL ERROR - WRITING TO FILE");
        mem_exit(fdata, data, file, fd);
        exit(-1);
    }

    // Reading file byte for byte to determine length
    lseek(fd, 0, SEEK_SET);

    while(1) {
        red = read(fd, &buf, 1);

        if(fd == -1) {
        fprintf(stderr, "FATAL ERROR - READING FILE");
        mem_exit(fdata, data, file, fd);
        exit(-1);
    }
        
        if(red != 0) {
            count++;
        } else {
            break;
        }
    }

    lseek(fd, 0, SEEK_SET);

    // Reading file based off upper calculations
    read(fd, fdata, count);
    if(fd == -1) {
        fprintf(stderr, "FATAL ERROR - READING FILE");
        mem_exit(fdata, data, file, fd);
        exit(-1);
    }

    printf("READ FILE DATA: %s\n", fdata);

    mem_exit(fdata, data, file, fd);

    return 0;
}

void mem_exit(char *fdata, char *data, char *file, int fd) {
    free(fdata);
    free(data);
    free(file);

    close(fd);
}
