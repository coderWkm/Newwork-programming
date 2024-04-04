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
        printf("Usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }     
    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}