# 第三章：地址族与数据序列
端口号是同一操作系统内为区分不同套接字而设置的。TCP套接字和UDP套接字不会共用端口号，所以可以重复（P39）
> 结合小林coding八股：https://xiaolincoding.com/network/3_tcp/port.html#tcp-%E5%92%8C-udp-%E5%8F%AF%E4%BB%A5%E5%90%8C%E6%97%B6%E7%BB%91%E5%AE%9A%E7%9B%B8%E5%90%8C%E7%9A%84%E7%AB%AF%E5%8F%A3%E5%90%97

P40页的内容给人以下提示：有类似于**跨操作系统（32bit-64bit）**的移植需求，尽量遵循**POSIX规范**

关于结构体`sockaddr_in`：
- 协议族的地址族：AF_INET(ipv4)
- 网络地址、端口号
- sin_zero：为了保证与`sockaddr`结构体大小一致而补充0

## 网络字节序、地址变换
网络字节序约定：**统一用大端序**

- 转换ip地址：htonl-ntohl
- 转换端口号：htons-ntohs
> h代表：host；n代表network；l是long，s是short。<br>
> 即使本身就是大端cpu，也建议使用上述函数进行转换。

- 网络地址初始化：inet_addr()/inet_aton
> P47提到`inet_aton`使用频率更高，但是全书一直使用前者进行地址转换？
- 地址转换为字符串：inet_ntoa()

P49下提到初始化`sockaddr_in`为0的目的：将成员`sin_zero`初始化为0

P50中：初始化该结构体的意义

初始化服务端的IP：INADDR_ANY


#### 分配套接字地址：bind()
```cpp
int bind(int socketfd, struct sockaddr*, socklen_t addrlen)

// 将第二个参数的结构体地址分配给第一个参数的套接字
```
