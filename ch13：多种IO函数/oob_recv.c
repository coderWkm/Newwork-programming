#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>

#define BUF_SIZE 30

void error_handling(const char error[]);
void urg_handler(int signo);

int recv_sock;
int acpt_sock;

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <Port>\n", argv[1]);
        exit(1);
    }     

    struct sockaddr_in recv_addr, serv_addr;
    char buf[BUF_SIZE];
    struct sigaction act;

    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (acpt_sock == -1) {
        error_handling("error: socket()");
    }
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(acpt_sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) == -1) {
        error_handling("error: bind()");
    }
    if (listen(acpt_sock, 5) == -1) {
        error_handling("error: listen()");
    }

    socklen_t serv_adr_sz = sizeof(serv_addr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_addr, &serv_adr_sz);

    fcntl(recv_sock, __F_SETOWN, getpid());
    int state = sigaction(SIGURG, &act, 0);

    int str_len = 0;
    while ((str_len = read(recv_sock, buf, sizeof(buf))) != 0) {
        if (str_len == -1) {
            continue;
        }
        buf[str_len] = '\0';
        puts(buf);
    }

    close(recv_sock);
    close(acpt_sock);

    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}

void urg_handler(int signo) 
{
    char buf[BUF_SIZE];
    int str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = '\0';
    printf("Urgent message: %s \n", buf);
}