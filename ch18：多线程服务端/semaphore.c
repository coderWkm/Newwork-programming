#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

int num = 0;
void* get_sum(void* args);
void* add(void* args);
sem_t sem_one;
sem_t sem_two;

int main(int argc, char* argv[])
{
    pthread_t thread_get_num, thread_add;
//    int cnt = 5, input;
  //  puts("Your input:");
    //scanf("%d\n", &input);
    
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);
    
    //sem_wait(&sem_one);
    pthread_create(&thread_get_num, NULL, get_sum, NULL);
    pthread_create(&thread_add, NULL, add, NULL);
    //sem_post(&sem_one);

    //sem_wait(&sem_two);
    pthread_join(thread_get_num, NULL);
    pthread_join(thread_add, NULL);
    //sem_post(&sem_one);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    //printf("num: %d\n", num);

    return 0;
}

void* get_sum(void* input)
{
    for (int i = 0; i < 5; ++i) {
        fputs("Input num:", stdout);
        sem_wait(&sem_two);
        scanf("%d", &num);
        sem_post(&sem_one);
    }
    return NULL;
}

void* add(void* args)
{
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        sem_wait(&sem_one);
        sum += num;
        sem_post(&sem_two);
    }
    printf("result: %d\n", sum);
    return NULL;
}