#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024

void error_handling(const char* message);
int main(int argc, char* argv[])
{
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in clnt_addr;
    int sock;

    if (argc != 3) {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("error: socket()");
    }

    memset(&clnt_addr, 0, sizeof(clnt_addr));
    clnt_addr.sin_family = AF_INET;
    clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
    clnt_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr)) == -1) {
        error_handling("error: connect");
    }
    else {
        puts("Connected........");
    }

    while (1) {
        fputs("Input message(Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "Q\n") || !(strcmp(message, "q\n"))) {
            break;
        }

        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE - 1);
        message[str_len] = 0;
        printf("Message from server: %s\n", message);
    }
    close(sock);
 
    return 0;
}

void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}