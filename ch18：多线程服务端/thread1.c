#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_main(void*);

int main(int argc, char* argv[])
{
    pthread_t tid;
    int args_num = 5;
    if (pthread_create(&tid, NULL, thread_main, (void*)&args_num)) {
        puts("pthread_create error()");
        return -1;
    }
    sleep(10);
    puts("end of main");

    return 0;
}

void* thread_main(void* args)
{
    int arg_num = *((int*)args);
    for (int i = 0; i < arg_num; ++i) {
        sleep(1);
        puts("running thread");
    }
}
