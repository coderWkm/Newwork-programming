# 半关闭套接字

方法： shutdown()
> 调用close会向对方发出EOF，但不能再接受对方发送的数据；
>
> 调用shutdown也会发出EOF，但是仍可以保留输入流（P120下）

