#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void error_handling(const char error[]);
void read_childproc(int sig);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <Port>\n", argv[0]);
        exit(1);
    }
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_adr_sz;
    int serv_sock, clnt_sock, epfd;
    char message[BUF_SIZE];
    //fd_set reads, copy_reads;
    // struct timeval timeout;    
    //int fd_num, fd_max, str_len;
    int str_len, event_cnt;
    struct epoll_event event;
    struct epoll_event* ep_events;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
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
        error_handling("error: listen()");
    }

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);
    while (1) {
        // 与ep_events申请的空间要一直        
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1); 
        if (event_cnt == -1) {
            puts("epoll_wait() error");
            break;
        }

        puts("return epoll_wait");
        for (int i = 0; i < event_cnt; ++i) {
            if (ep_events[i].data.fd == serv_sock) {
                clnt_adr_sz = sizeof(clnt_addr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client: %d\n", event.data.fd);
            }
            else {
                str_len = read(ep_events[i].data.fd, message, BUF_SIZE - 1);
                if (!str_len) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("Closed client: %d\n", ep_events[i].data.fd);
                }
                else {
                    write(ep_events[i].data.fd, message, str_len);
                }
            }
        }
    }

    close(serv_sock);
    close(epfd);
        

    // FD_ZERO(&reads);
    // FD_SET(serv_sock, &reads);
    // fd_max = serv_sock;
    
    // while (1) {
    //     copy_reads = reads;
    //     timeout.tv_sec = 5;
    //     timeout.tv_usec = 5000;

    //     if ((fd_num = select(fd_max + 1, &copy_reads, 0, 0, &timeout)) == -1) {
    //         break;
    //     }
    //     if (!fd_num) {
    //         puts("time-out.");
    //         continue;
    //     }

    //     for (int i = 0; i < fd_max + 1; ++i) {
    //         if (FD_ISSET(i, &copy_reads)) {
    //             if (i == serv_sock) {
    //                 clnt_adr_sz = sizeof(clnt_addr);
    //                 int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
    //                 FD_SET(clnt_sock, &reads);
    //                 if (fd_max < clnt_sock) {
    //                     fd_max = clnt_sock;
    //                 }
    //                 printf("connected client: %d \n", clnt_sock);
    //             }
    //             else {
    //                 str_len = read(i, message, BUF_SIZE);
    //                 if (str_len == 0) {
    //                     FD_CLR(i, &reads);
    //                     close(i);
    //                     printf("closed client: %d \n", i);
    //                 }
    //                 else {
    //                     write(i, message, str_len);
    //                 }
    //             }
    //         }
    //     }
        
    // }

    // int fds[2];
    // pipe(fds);
    // pid = fork();
    // if (!pid) {
    //     FILE *fp = fopen("echomsg.txt", "wt");
    //     char msgbuf[BUF_SIZE];
    //     if (!fp) {
    //         error_handling("error: fopen");
    //     }
        
    //     for (int i = 0; i < 10; ++i) {
    //         int len = read(fds[0], msgbuf, BUF_SIZE);
    //         fwrite(msgbuf, 1, len, fp);
    //     }
    //     fclose(fp);
    //     return 0;
    // }

    // while (1) {
    //     clnt_adr_sz = sizeof(clnt_addr);
    //     int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
    //     if (clnt_sock == -1)  {
    //         continue;
    //     }
    //     else {
    //         puts("new client connectted...");
    //     }
        
    //     pid = fork();
    //     if (pid == -1) {
    //         close(clnt_sock);
    //         continue;
    //     }
    //     if (!pid) {
    //         close(serv_sock);

    //         str_len = 0;
    //         while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
    //             write(clnt_sock, message, str_len);
    //             write(fds[1], message, str_len);
    //         }

    //         close(clnt_sock);
    //         puts("client disconnected...");
    //         return 0;
    //     }
    //     else {
    //         close(clnt_sock);
    //     }
    // }
    close(serv_sock);
    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}
// void read_childproc(int sig) 
// {
//     int status;
//     pid_t pid = waitpid(-1, &status, WNOHANG);
//     printf("Child process num: %d\n", pid);
// }