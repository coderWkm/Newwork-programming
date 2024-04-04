#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(const char error[]);

int main(int argc, char* argv[])
{
    int set_buf = 1024 * 3;
    int snd_buf = 0, rcv_buf = 0;
    int sock;
    int state;
    socklen_t len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    len = sizeof(set_buf);
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&set_buf, sizeof(set_buf));
    if (state == -1) {
        error_handling("error: setsockopt---->snd_buf");
    }
    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if (state == -1) {
        error_handling("error: getsockopt---->snd_buf");
    }

    len = sizeof(set_buf);
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&set_buf, sizeof(set_buf));
    if (state == -1) {
        error_handling("error: setsockopt---->rcv_buf");
    }
    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if (state == -1) {
        error_handling("error: getsockopt---->rcv_buf");
    }

    printf("send buff(set): %d\n", snd_buf);
    printf("receive buff(set): %d\n", rcv_buf);
    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}