# I/O复用
多进程的缺点：创建进程、进程上下文切换很占用内存和运算，并且IPC会提高编程的难度

## 服务端的复用：select
多进程服务端：一个客户端对应一个进程；

I/O服用：服务端只有一个进程

select是将多个文件描述符集中起来统一监视，每个监视项成为`事件`，发生监视项的对应情况时则称之为`发生了事件`

使用select函数的顺序：

![](https://wkm-img-1313869410.cos.ap-chongqing.myqcloud.com/images/202403231506251.png)

### 设置文件描述符
将所有文件描述符集中到`fd_set`数组中，每个项对应一个套接字的fd，如果被设置为1，则是监视对象。

设置方法，几个宏：
- FD_ZERO(fd_set* fdset)：将fdset所有位初始化位0
- FD_SET(int fd, fd_set* fdset)：注册fd
- FD_CLR(int fd, fd_set* fdset)：清除fd
- FD_ISSET(int fd, fd_set* fdset)：fd注册了则返回true

select函数：
```cpp
#include <sys/select.h>
#include <sys/time.h>

int select(
    int maxfd, fd_set* readset, fd_set* writeset, fd_set* exceptset,
    const struct timeval* timeout
)
/**
    分别为：监视的分拣描述符数量、读set、写set、异常set、超时的结构体
    // 每次新建描述符时，maxfd的数值会+1，因此需要传入 `最大的文件描述符 + 1`

    返回值：
        1. 发生错误返回-1
        2. 超时返回0
        3. 发生关注事件时， 返回发生事件的文件描述符数量
***/

struct timeval{
    long tv_sec;    //秒
    long tv_usec;   //微妙
};
```
select返回值大于0时，判断哪些fd发生了事件：原来为1的变为0， **发生事件的fd仍为1**
