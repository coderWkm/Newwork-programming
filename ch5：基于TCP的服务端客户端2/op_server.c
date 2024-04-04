#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4
#define RLT_SIZE 4

void error_handling(const char* message);
int compute(int op_num, int mes[], char op);


int main(int argc, char* argv[])
{
    struct sockaddr_in serv_addr, clnt_addr;
    int serv, clnt;
    socklen_t clnt_adr_sz;
    int mes_len;
    char message[BUF_SIZE];

    if (argc != 2) {
        printf("Usage:%s <Port>\n", argv[0]);
        exit(1);
    }
    
    serv = socket(PF_INET, SOCK_STREAM, 0);
    if (serv == -1) {
        error_handling("error: socket()");
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
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

        int op_count;
        read(clnt, &op_count, 1);
        mes_len = 0; 
        int recv_cnt = 0;
        while (mes_len < (op_count * OPSZ + 1)) {
            recv_cnt = read(clnt, &message[mes_len], BUF_SIZE - 1);
            mes_len += recv_cnt;
        }
        int result = compute(op_count, (int *)message, message[mes_len - 1]);
        write(clnt, (char *)&result, sizeof(result));
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

int compute(int op_num, int mes[], char op) 
{
    int result = mes[0];
    switch(op) {
        case '+' : 
            for (int i = 1; i < op_num; ++i) {
                result += mes[i];
            }
            break;
        case '-' : 
            for (int i = 1; i < op_num; ++i) {
                result -= mes[i];
            }
            break;
        case '*' : 
            for (int i = 1; i < op_num; ++i) {
                result *= mes[i];
            }
            break;
        case '/' : 
            for (int i = 1; i < op_num; ++i) {
                result /= mes[i];
            }
            break;
    }
    return result;
}