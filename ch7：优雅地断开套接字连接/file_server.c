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
        printf("Usage: %s <Port>\n", argv[0]);
        exit(1);
    }   

    struct sockaddr_in serv_addr, clnt_addr;
    char message[BUF_SIZE];

    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("error: socket()");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("error: bind()");
    }
    if (listen(serv_sock, 5) == -1) {
        error_handling("error: listen");
    }

    socklen_t clnt_adr_sz = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
    FILE* fp = fopen("file_server.c", "rb"); 
    if (!fp) {
        error_handling("error: fopen()");
    }
    while (1) {
        int read_cnt = fread((void*)message, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE) {
            write(clnt_sock, message, read_cnt);
            break;
        }
        write(clnt_sock, message, BUF_SIZE);
    }
    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, message, BUF_SIZE);
    //message[read_cnt] = '\0';
    printf("Message from client: %s\n", message);
    
    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}