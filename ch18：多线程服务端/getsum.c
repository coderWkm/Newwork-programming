#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* thread_sum(void*);
int sum = 0;

int main(int argc, char* argv[])
{
    pthread_t tid1, tid2;
    //int args_num = 5;
    //void *thr_ret1, *thr_ret2;
    int arg1[] = {1, 2, 3, 4, 5};
    int arg2[] = {6, 7, 8, 9, 10};

    if (pthread_create(&tid1, NULL, thread_sum, (void*)arg1)) {
        puts("pthread_create1 error()");
        return -1;
    }
    if (pthread_join(tid1, NULL) != 0) {
        puts("pthread_join error()");
        return -1;
    }
    if (pthread_create(&tid2, NULL, thread_sum, (void*)arg2)) {
        puts("pthread_create2 error()");
        return -1;
    }
    if (pthread_join(tid2, NULL) != 0) {
        puts("pthread_join error()");
        return -1;
    }

    printf("1-10 sum: %d\n", sum);
    //free(thr_ret);

    return 0;
}

void* thread_sum(void* args)
{
    // int size = sizeof((int*)args) / sizeof(int);
    // printf("size: %d\n", size);
    for (int i = 0; i < 5; ++i) {
        sum += ((int*)args)[i];
    }
    return NULL;
}