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
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_adr_sz;
    int serv, clnt;
    char message[BUF_SIZE];

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv = socket(PF_INET, SOCK_STREAM, 0);
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

    if (listen(serv, 5) == -1) {
        error_handling("error: listen()");
    }

    clnt_adr_sz = sizeof(clnt_addr);
    for (int i = 0; i < 5; ++i) {
        clnt = accept(serv, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
        if (clnt == -1) {
            error_handling("error: accept()");
        }
        else {
            printf("Connected client %d \n", i + 1);
        }
        int mes_len = 0;
        while ((mes_len = read(clnt, message, BUF_SIZE)) != 0) {
            write(clnt, message, mes_len);
        }
        
        close(clnt);
    }
    close(serv);

    return 0;
}

void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}