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
        printf("Usage: %s <IP> <Port>\n", argv[1]);
        exit(1);
    }    
    char message1[] = "Hi!";
    char message2[] = "UDP host!";
    char message3[] = "Nice to meet you!";

    struct sockaddr_in sock_addr;
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("error: socket()");
    } 
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_addr.sin_port = htons(atoi(argv[2]));

    sendto(sock, message1, sizeof(message1), 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr));    
    sendto(sock, message2, sizeof(message2), 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr));    
    sendto(sock, message3, sizeof(message3), 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr));    
    close(sock);

    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}