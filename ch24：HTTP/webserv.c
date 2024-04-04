#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void error_handling(const char error[]);
void* request_handler(void*);
void send_error(FILE*);
char* content_type(char*);
void send_data(FILE*, char*, char*);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <Port>\n", argv[0]);
        exit(1);
    }     

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_adr_sz;
    pthread_t tid;

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
    // 前面说过web服务器的队列大小至少是20
    if (listen(serv_sock, 20) == -1) {
        error_handling("error: listen()");
    }

    while (1) {
        clnt_adr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_adr_sz);
        if (clnt_sock == -1) {
            error_handling("error: accept()");
        }
        printf("Connect request: %s:%d\n", 
                inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
        pthread_create(&tid, NULL, request_handler, (void*)&clnt_sock);
        pthread_detach(tid);
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

void* request_handler(void* args)
{
    int clnt_sock = *((int*)args);
    char small_buf[SMALL_BUF];
    char method[10];
    char ct[10];
    char file_name[10];

    FILE *clnt_read, *clnt_write;
    clnt_read = fdopen(clnt_sock, "r");
    clnt_write = fdopen(dup(clnt_sock), "w");
    fgets(small_buf, SMALL_BUF, clnt_read);
    
    if (!strstr(small_buf, "HTTP/")) {
        send_error(clnt_write);
        fclose(clnt_write);
        fclose(clnt_read);
        return NULL;
    }

    strcpy(method, strtok(small_buf, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));
    if (strcmp(method, "GET") != 0) {
        send_error(clnt_write);
        fclose(clnt_write);
        fclose(clnt_read);
        return NULL;
    }
    fclose(clnt_read);
    send_data(clnt_write, ct, file_name);
    return NULL;
}

void send_error(FILE* fp)
{
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server : Linux Web Server \r\n";
    char cnt_len[] = "Content-length: 2048\r\n";
    char cnt_type[] = "Content-type: text/html\r\n\r\n";
    // 消息头和消息体多插入一个空行

    char content[] = "<html><head><title>NETWORK></head>"
                     "<body><font size=+5><br>发生错误！查看请求文件名和请求方式！ "
                     "</font></body></html>";
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content, fp);
    fflush(fp);
}

char* content_type(char* file)
{
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];

    strcpy(file_name, file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));

    if (!strcmp(extension, "html") || !strcmp(extension, "htm")) {
        return "text/html";
    }
    else {
        return "text/plain"; 
    }
}

void send_data(FILE* fp, char* ct, char* file_name)
{
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char server[] = "Server : Linux Web Server \r\n";
    char cnt_len[] = "Content-length: 2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    FILE* send_file = fopen(file_name, "r");
    if (!send_file) {
        send_error(fp);
        return ;
    }

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    while (fgets(buf, BUF_SIZE, send_file) != NULL) {
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
}