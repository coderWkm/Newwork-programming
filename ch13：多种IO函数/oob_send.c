#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(const char error[]);

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage:%s <IP> <Port>\n", argv[0]);
    }
    struct sockaddr_in recv_addr;
    int sock;
    char buf[BUF_SIZE];

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("error: socket()");
    }
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    recv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1) {
        error_handling("error: connect()");
    }

    write(sock, "123", strlen("123"));
    send(sock, "4", strlen("4"), MSG_OOB);
    write(sock, "567", strlen("567"));
    send(sock, "890", strlen("890"), MSG_OOB);
    close(sock);

    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}