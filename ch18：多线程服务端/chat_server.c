#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>

/**
 * 
 * 暂存问题：客户端发送消息时，服务端的终端会无限输出该信息
*/

#define BUF_SIZE 100
#define MAX_CLNT 256
int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutex;

void error_handling(const char error[]);
void send_msg(char* msg, int len);
void* handle_clnt(void* sock);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <Port>\n", argv[0]);
        exit(1);
    }     
    
    struct sockaddr_in serv_addr, clnt_addr;
    int serv_sock, clnt_sock;
    socklen_t clnt_adr_sz;
    pthread_t t_id;
    
    pthread_mutex_init(&mutex, NULL);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("error: socket()");
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("error: bind()");
    }
    if (listen(serv_sock, 5) == -1) {
        error_handling("error: listen()");
    }

    while (1) {
        clnt_adr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);

        pthread_mutex_lock(&mutex);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id);
        printf("connected client IP:%s\n", inet_ntoa(clnt_addr.sin_addr));
    }
    
    close(serv_sock);
    
    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}

void send_msg(char* msg, int len) 
{
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLNT; ++i) {
        write(clnt_socks[i], msg, len);
    }
    pthread_mutex_unlock(&mutex);
}

void* handle_clnt(void* sock)
{
    int clnt = *((int *)sock);
    char msg[BUF_SIZE];
    int len = 0;

    while ((len = read(clnt, msg, BUF_SIZE - 1)) != 0) {
        send_msg(msg, len);
    }

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < clnt_cnt; ++i) {
        if (clnt_socks[i] == clnt) {
            while (i++ < clnt_cnt-1) {
                clnt_socks[i] = clnt_socks[i + 1];
            }
            break;
        }
    }
    --clnt_cnt;
    pthread_mutex_unlock(&mutex);
    close(clnt);
    return NULL;
}