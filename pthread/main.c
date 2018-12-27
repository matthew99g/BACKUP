#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int sum = 0;

void *threadProcess(void *arg) {
    const int MAX_NUM = *(int *) arg;
    for(int i = 0; i < MAX_NUM; i++) {
        sum++;
    }

    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    int num = 500;

    pthread_t tid;

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid, &attr, threadProcess, &num);

    pthread_join(tid, NULL);

    printf("Sum: %d", sum);
    return 0;
}
