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
    
    struct sockaddr_in sock_addr;
    char message[BUF_SIZE];

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("error: socket()");
    }
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(argv[1]);
    sock_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1) {
        error_handling("error: connect()");
    }    

    int read_cnt = 0;
    FILE* fp = fopen("save.dat", "wb");
    while ((read_cnt = read(sock, message, BUF_SIZE)) != 0) {
        fwrite(message, 1, read_cnt, fp);
    }

    puts("Received file data");
    write(sock, "Thank you", 10);
    fclose(fp);
    close(sock);
    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}