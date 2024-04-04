#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void* thread_main(void*);

int main(int argc, char* argv[])
{
    pthread_t tid;
    int args_num = 5;
    void* thr_ret;

    if (pthread_create(&tid, NULL, thread_main, (void*)&args_num)) {
        puts("pthread_create error()");
        return -1;
    }
    if (pthread_join(tid, &thr_ret) != 0) {
        puts("pthread_join error()");
        return -1;
    }

    printf("Thread return message: %s\n", (char*)thr_ret);
    free(thr_ret);

    return 0;
}

void* thread_main(void* args)
{
    int arg_num = *((int*)args);
    char *msg = malloc(sizeof(char) * 50);
    strcpy(msg, "Hi! The message from thread.");
    for (int i = 0; i < arg_num; ++i) {
        sleep(1);
        puts("running thread");
    }
    return (void*)msg;
}
