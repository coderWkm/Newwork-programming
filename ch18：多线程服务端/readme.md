# 多线程服务端
每个进程都有独立的内存空间：全局变量区、malloc使用的堆、栈等

## 线程创建、使用
```cpp
#include <pthread.h>

int pthread_create(pthread_t * restrict thread, 
                   const pthread_attr_t* restrict attr,
                   void* (* start_routine)(void *), 
                   void* restrict arg);
/**
    - thread：线程id地址
    - attr：线程属性，不用的话传递NULL
    - start_routine：线程的main函数，参数和返回值均为void*
    - arg：传递给线程main的参数
    // 用法简单，但是需要熟练掌握restrict和函数指针！
*/

int pthread_join(pthread_t thread, void** status);
/**
    - 调用该函数的线程进入等待状态，知道ID为thread的线程终止
    - status：线程main的返回值
*/
```

大多数标准函数是线程安全的函数；一些线程安全的函数带有`_r`的后缀

编译多线程的代码，添加：
- 宏：_REENTRANT
- 链接pthread库： -lpthread

## 线程同步
### 互斥锁
```cpp
#include <pthread.h>

// 创建地址为mutex的互斥锁，属性不使用传入nullptr
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);
//销毁互斥锁
int pthread_mutex_destroy(pthread_mutex_t* mutex);
// 加锁和解锁
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);
```
### 信号量
```cpp
#include <semaphore.h>
/**
    - sem：信号量地址
    - pshared：0表示只允许在一个进程内部使用；否则信号量可被多个进程共享
    - value：信号量初始值
**/
int sem_init(sem_t * sem, int pshared, unsigned int value);
int set_destroy(set_t *set);

// +1/-1
int set_post(sem_t * sem);
int set_wait(sem_t * sem);
```

### 推荐的销毁线程方式

```c
int pthread_detach(pthread_t thread);
/*
    pthread_join：在线程退出后也会引导线程销毁，但此前调用该函数的线程处于阻塞
*/
```