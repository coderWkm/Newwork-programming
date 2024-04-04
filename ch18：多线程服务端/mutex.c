#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREAD_NUM 100
#define ITER_NUM 5000
long long num = 0;
void* thread_inc();
void* thread_dec();
pthread_mutex_t mutex;

int main(int argc, char* argv[])
{
    pthread_t threads[THREAD_NUM];
    pthread_mutex_init(&mutex, NULL);

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

    pthread_mutex_destroy(&mutex);
    return 0;
}

void* thread_inc() {
    int i = 0;
    for (; i < ITER_NUM; ++i) {
        pthread_mutex_lock(&mutex);
        num += 1;
        pthread_mutex_unlock(&mutex);
    }
}
void* thread_dec() {
    int i = 0;
    for (; i < ITER_NUM; ++i) {
        pthread_mutex_lock(&mutex);
        num -= 1;
        pthread_mutex_unlock(&mutex);
    }
}