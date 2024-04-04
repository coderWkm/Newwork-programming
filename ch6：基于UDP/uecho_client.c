#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(const char error[]);

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage:%s <Ip> <Port>\n", argv[0]);
        exit(1);
    }

    struct sockaddr_in sock, from_addr;
    char message[BUF_SIZE];

    int sock_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1) {
        error_handling("error: socket()");
    }     
    memset(&sock, 0, sizeof(sock));
    sock.sin_family = AF_INET;
    //sock.sin_addr.s_addr = htonl(inet_aton(argv[1], &sock.sin_addr));
    inet_aton(argv[1], &sock.sin_addr);
    sock.sin_port = htons(atoi(argv[2]));

    while (1) {
        fputs("Input message(Q/q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if (!strcmp(message, "Q\n") || !strcmp(message, "q\n")) {
            break;
        } 

        socklen_t from_adr_sz = sizeof(from_addr);
        sendto(sock_fd, message, strlen(message), 0, (struct sockaddr*)&sock, sizeof(sock));
        int str_len = recvfrom(sock_fd, message, BUF_SIZE, 0, (struct sockaddr*)&from_addr, &from_adr_sz);
        //message[str_len] = '\0';
        printf("Message from server: %s\n", message);
    }
    close(sock_fd);

    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}