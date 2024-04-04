#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(const char error[]);

int main(int argc, char* argv[])
{
    int snd_buf, rcv_buf;
    int sock;
    int state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if (state == -1) {
        error_handling("error: getsockopt---->snd_buf");
    }

    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if (state == -1) {
        error_handling("error: getsockopt---->rcv_buf");
    }

    printf("send buff(default): %d\n", snd_buf);
    printf("receive buff(default): %d\n", rcv_buf);
    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}