/*
 * pthread.h Example
 * By: Matthew Todd Geiger #214 
 * Date: 10/17/2018
 * 
 * I Demonstrate how to share chunks of memory to multiple threads using
 * struct arrays to hold data.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct t_data {
    int sum;
    int answer;
    int iTime;
};

// Thread process must be a void function pointer with a void pointer as its argument
void *threadProcess(void *arg) {
    struct t_data *tData = (struct t_data *) arg;
    tData->answer = 0;

    for(int i = 0; i < tData->iTime; i++)
        tData->answer += tData->sum;

    pthread_exit(0);
}

int main(int argc, char const *argv[])
{
    // Check to make sure there is atleast one extra parameter
    if(argc < 2) {
        fprintf(stderr, "%s <NUM> <NUM> <NUM>...", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Argument counter starting at 0
    const int ARGCV = argc - 1;

    // Array's for thread memory
    pthread_t tid[ARGCV];
    struct t_data threadData[ARGCV];

    // Gather thread attributes for thread creation
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Loop through assigning data to structs and creating threads
    for(int i = 0; i < ARGCV; i++) {
        threadData[i].iTime = 100000000;
        threadData[i].sum = atoi(argv[i + 1]);

        pthread_create(&tid[i], &attr, threadProcess, &threadData[i]);
    }

    // Loop through to check all thread completion
    for(int i = 0; i < ARGCV; i++) {
        pthread_join(tid[i], NULL);
        printf("%d\n\n", threadData[i].answer);
    }
    
    return 0;
}
