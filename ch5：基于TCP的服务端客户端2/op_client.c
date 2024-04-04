#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(const char* message);

int main(int argc, char* argv[])
{
    struct sockaddr_in clnt;
    int sock;
    int op_count, result;
    char message[BUF_SIZE];
    char operator;

    if (argc != 3) {
        printf("Usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("error: socket()");
    }
    
    memset(&clnt, 0, sizeof(clnt));
    clnt.sin_addr.s_addr = inet_addr(argv[1]);
    clnt.sin_family = AF_INET;
    clnt.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr*)&clnt, sizeof(clnt)) == -1) {
        error_handling("error: connect()");
    }
    else {
        printf("Connected....\n");
    }

    // 怎么发过去，再收回来结果？"
    fputs("Operand count:", stdout);
    scanf("%d", &op_count);
    message[0] = (char)op_count;
    
    for (int i = 0; i < op_count; ++i) {
        printf("Operand %d:", i + 1);
        scanf("%d", (int *)&message[i * OPSZ + 1]);
    }
    fgetc(stdin);
    fputs("Operator:", stdout);
    scanf("%c", &message[op_count * OPSZ + 1]);
    write(sock, message, op_count * OPSZ + 2);
    read(sock, &result, RLT_SIZE);
    
    printf("Operation result: %d\n", result);
    close(sock);


    return 0;
}

void error_handling(const char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}