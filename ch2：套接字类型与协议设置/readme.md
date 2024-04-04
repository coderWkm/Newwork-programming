# 第二章：套接字类型和协议设置

### 创建套接字：socket()
```cpp
#include <sys/socket.h>

int socket(int domain, int type, int protocol);

/**
 * 分别为：
    协议族类型、
    套接字数据传输类型、
    计算机通信使用的协议
 * */ 
```
关于第二个参数TCP和UDP的区别：

- TCP套接字的read/write次数不一定一一对应；
- UDP数据有边界，读写次数必须一致；
