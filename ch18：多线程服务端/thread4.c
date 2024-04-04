#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUM 100
#define ITER_NUM 50000000
long long num = 0;
void* thread_inc();
void* thread_dec();

int main(int argc, char* argv[])
{
    pthread_t threads[THREAD_NUM];
    printf("long long size: %lu\n", sizeof(long long));
    for (int i = 0; i < THREAD_NUM; ++i) {
        if (i % 2) {
            pthread_create(&threads[i], NULL, thread_inc, NULL);
        }
        else {
            pthread_create(&threads[i], NULL, thread_dec, NULL);
        }
    }
    for (int i = 0; i < THREAD_NUM; ++i) {
        pthread_join(threads[i], NULL);
    }
    printf("num: %lld\n", num);

    return 0;
}

void* thread_inc() {
    int i = 0;
    for (; i < ITER_NUM; ++i) {
        num += 1;
    }
}
void* thread_dec() {
    int i = 0;
    for (; i < ITER_NUM; ++i) {
        num -= 1;
    }
}