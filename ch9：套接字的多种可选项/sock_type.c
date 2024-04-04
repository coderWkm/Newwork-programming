#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(const char error[]);

int main(int argc, char* argv[])
{
    int tcp_sock, udp_sock;
    int sock_type;
    socklen_t oplen;
    int state;

    oplen = sizeof(sock_type);
    tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    printf("TCP:%d\n ", SOCK_STREAM);
    printf("UDP:%d\n ", SOCK_DGRAM);
    
    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &oplen);
    if (state == -1) {
        error_handling("error: getsockopt()---tcp");
    }
    printf("Socket type one:%d \n", sock_type);

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &oplen);
    if (state == -1) {
        error_handling("error: getsockopt()---udp");
    }
    printf("Socket type two:%d \n", sock_type);
    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}