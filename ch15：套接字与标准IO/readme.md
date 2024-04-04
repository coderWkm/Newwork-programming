# 套接字与标准I/O
优点：
1. 标准I/O更具备可移植性
2. 标准I/O拥有额外的缓冲，提高性能，而且传输数据越多，有无缓冲的性能差异越大

缺点：
1. 不容易双向通信
2. 频繁调用fflush
3. 需要以FILE指针形式返回文件描述符

## 标准I/O
```cpp
#include <stdio.h>

FILE* fdopen(int fd, const char* mode);

// mode与fopen打开文件的方式相同


int fileno(FILE* stream);
//返回转换后的文件描述符
```
> 通过P252示例可知：
> - 在回声服务端写回信息后，需要fflush, 保证立即传回客户端； 
> - 使用完读写FILE*后需要调用`fclose`

