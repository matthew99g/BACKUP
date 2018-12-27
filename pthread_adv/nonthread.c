#include <stdio.h>
#include <stdlib.h>

int normalProcess(int sum, int iTime) {
    int answer = 0;

    for(int i = 0; i < iTime; i++)
        answer += sum;

    return answer;
}

int main(int argc, char const *argv[])
{
    if(argc < 2) {
        fprintf(stderr, "%s <NUM> <NUM> <NUM>...", argv[0]);
        exit(EXIT_FAILURE);
    }

    const int ARGCV = argc - 1;

    for(int i = 0; i < ARGCV; i++) {
        // printf("\n%d\n", atoi(argv[i + 1]));
        int result = normalProcess(atoi(argv[i + 1]), 100000000);
        printf("%d\n", result);
    }

    return 0;
}
