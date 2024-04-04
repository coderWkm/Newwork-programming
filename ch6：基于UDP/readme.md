# 基于UDP的服务端/客户端
UDP服务端、客户端均只需1个套接字

主要是I/O函数：sendto/recvfrom
> 同时在申请套接字时传入对应的参数
>
> 调用sendto时自动分配IP和端口号（P109）

通过sendto函数发送的三个阶段：P112；最耗时的是一、三阶段（注册IP和端口、删除这些信息）。建立connected UDP套接字：
- **使用connect()**; write/read替换sendto和recvfrom

