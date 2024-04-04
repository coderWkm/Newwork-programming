#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20
char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

void* send_msg(void*);
void* recv_msg(void*);
void error_handling(const char error[]);

int main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("Usage: %s <IP> <Port> <name>\n", argv[0]);
        exit(1);
    }
    sprintf(name, "[%s]", argv[3]);

    int sock;
    struct sockaddr_in serv_addr;
    pthread_t snd_tid, rcv_tid;
    void* thread_return;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("error: socket()");
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("error: connect()");
    }

    pthread_create(&snd_tid, NULL, send_msg, (void*)&sock); 
    pthread_create(&rcv_tid, NULL, recv_msg, (void*)&sock); 
    pthread_join(snd_tid, &thread_return);
    pthread_join(rcv_tid, &thread_return);

    close(sock);

    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}

void* send_msg(void* arg)
{
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];

    while (1) {
        fgets(msg, BUF_SIZE, stdin);
        if (!strcmp(msg, "Q\n") || !strcmp(msg, "q\n")) {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void* recv_msg(void* arg)
{
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE];

    while (1) {
        int str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);
        if (str_len == -1) {
            return (void*)-1;
        }
        name_msg[str_len] = '\0';
        fputs(name_msg, stdout);
    } 
    return NULL;
}