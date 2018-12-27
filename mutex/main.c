#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct p_data {
    int iSum;
    int iTime;
};

void *threadProcess(void *arg) {
    struct p_data *pdata = (struct p_data*) arg;

    for(int i = 0; i < pdata->iTime; i++) {
        pthread_mutex_lock(&mutex);

        pdata->iSum++;

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    const int ARGCV = 2;

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    struct p_data pdata;
    pthread_t tids[ARGCV];

    pdata.iTime = 10000000;
    pdata.iSum = -10000000;

    for (int i = 0; i < ARGCV; i++)
        pthread_create(&tids[i], &attr, threadProcess, &pdata);

    for (int i = 0; i < ARGCV; i++)
        pthread_join(tids[i], NULL);

    printf("iSum: %d\n", pdata.iSum);

    return 0;
}
