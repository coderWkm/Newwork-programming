#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(const char error[]);

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    struct sockaddr_in serv_addr, clnt_addr;
    char message[BUF_SIZE];

    int serv = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv == -1) {
        error_handling("error: socket()");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("error: bind()");
    }

    while(1) {
        socklen_t clnt_adr_sz = sizeof(clnt_addr);
        int recv_len = recvfrom(serv, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
        sendto(serv, message, recv_len, 0, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));
    }
    
    close(serv);

    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}