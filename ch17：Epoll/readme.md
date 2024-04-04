# epoll
epoll技术是Linux特定的。

select性能缺陷：
1. 主要：每次循环向操作系统传递新的fd_set信息
2. 循环监视fd_set的每个文件描述符

## epoll的函数和结构体
epoll自linux内核2.5.44引入
```cpp
#include <sys/epoll.h>

//返回epoll文件描述符，传入参数为每个epoll实例的大小
//但是参数值是只给操作系统的建议
int epoll_create(int size);

/*
“epoll例程”：epoll_create返回值; 
功能：向epoll例程中注册监视对象文件描述符

op参数：
    - EPOLL_CTL_ADD：将文件描述符注册到例程
    - EPOLL_CTL_DEL：从例程中删除文件描述符（第四个参数为NULL）
    - EPOLL_CTL_MOD：更改注册的文件描述符的关注事件发生情况
**/
int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);


/*
event成员：
    - EPOLLIN：需要读取数据
    - EPOLLET：边缘触发的方式通知事件
    其他见P270
*/
struct epoll_event 
{
    __unit32_t events;
    epoll_data_t data;
}

typedef union epoll_data 
{
    void* ptr;
    int fd;
    __unit32_t u32;
    __unit64_t u64;
}epoll_data_t;

// 声明足够大的epoll_event数组，传递给epoll_wait后，发生变化的文件描述符信息填入该数组
/*
    - events：需要动态分配
    - maxevents：第二参数可以保存的最大事件数量
    - timeout：毫秒为单位的等待时间；-1代表一直到事件发生
    返回值：发生事件的文件描述符数量
*/
int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);
```

## 条件触发与边缘触发
- 条件出发：只要符合条件，会一直注册并通知该事件
- 边缘触发：仅注册1次

epoll默认的方式是条件出发


### 边缘触发的两点
1. 通过errno变量验证错误原因
> errno变量的头文件`error.h`；
>
>缓冲无数据可读时read返回-1，并在errno中保存`EAGAIN`常量

2. 更改套接字特性为非阻塞I/O。因为阻塞方式工作的read/write可能引起服务端的长时间停顿，边缘触发一定要采用非阻塞方式
```cpp
#include <fcntl.h>

// 返回cmd参数的相关值；此函数的用途很广泛！
int fcntl(int fd, int cmd, ...);

//更改套接字为非阻塞模式：
int flag = fcntl(fd, F_GETFL, 0);
fcntl(fd, F_SETFL, flag | O_NONBLOCK);
```

关于边缘触发的优势：服务端可以决定读取数据和处理数据的时间点，不能简单认为边缘触发一定会提高速度