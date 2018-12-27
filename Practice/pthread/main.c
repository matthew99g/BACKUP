#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct threadMemory {
    int answer;
    int sum;
    int times;
};

void *threadProcess(void *args) {
    struct threadMemory *tMem = (struct threadMemory *) args;

    for(int i = 0; i < tMem->times; i++) {
        tMem->answer += tMem->sum;
    }

    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "%s <num> <num> <num>...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const int threadAmount = argc - 1;

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_t tids[threadAmount];
    struct threadMemory tMem[threadAmount];

    for(int i = 0; i < threadAmount; i++) {
        int offset = i + 1;
        tMem[i].times = 500;
        tMem[i].sum = atoi(argv[offset]);
        pthread_create(&tids[i], &attr, threadProcess, &tMem[i]);
    }

    for(int i = 0; i < threadAmount; i++) {
        pthread_join(tids[i], NULL);
        printf("Calculated: %d\n", tMem[i].answer);
    }

    return EXIT_SUCCESS;
}