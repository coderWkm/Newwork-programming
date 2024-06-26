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
    if (argc != 2) {
        printf("Usage: %s <Port>\n", argv[1]);
        exit(1);
    }    
    
    struct sockaddr_in my_addr, your_addr;
    char message[BUF_SIZE];

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("error: socket()");
    }
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1) {
        error_handling("error: bind()");
    }

    for (int i = 0; i < 3; ++i) {
        sleep(2);
        socklen_t your_adr_sz = sizeof(your_addr);
        int str_len = recvfrom(sock, message, BUF_SIZE, 0, 
                              (struct sockaddr*)&your_addr, &your_adr_sz);
        //message[str_len] = '\0';
        printf("Message %d: %s\n", i + 1, message);
    }
    close(sock);

    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}