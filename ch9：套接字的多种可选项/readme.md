# 套接字的多种可选项
可选项：P140-141

- 读取套接字可选项：getsockopt()
- 设置套接字可选项：setsockopt()
## 1. 更改套接字的缓冲
通过SO_SNDBUF/SO_RCVBUF设置I/O缓冲大小，但系统不会完全按照要求进行设置（P145示例结果）

## 2. SO_REUSEADDR
P146案例：如果先关掉服务器（Ctrl+C），此时是服务器先发送FIN，因此服务器会保持一个`TIME_WAIT`状态，大约3min，如果在这3min内重启服务器会产生`bind() error`，即端口冲突。
> 客户端的端口是在connect函数内部随机分配P69

SO_REUSEADDR：将TIME_WAIT时占用的端口号重新分配

## 3. TCP_NODELAY
Nagle算法：
- 应用于TCP层，TCP套接字默认使用Nagle算法。
- 简单说：收到前一个消息的ACK时，才会将I/O缓冲的后一消息发出（发送前一消息后的等待过程中就将剩下的消息传到缓冲中）
- 关闭Nagle：发送与接收数据与ACK消息无关，几个消息就对应几个数据包（包括发送的TCP头部），还有接收的ACK
- 通过P151分析：网络过于拥堵建议使用Nagle，否则，对于某些情况（传输大文件，避免等待ACK的时间）关闭Nagle更好。**武断地关闭或者打开Nagle都是不合理的**

禁用方法：TCP_NODELAY改为1
