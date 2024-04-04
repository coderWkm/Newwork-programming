#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(const char error[]);

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <Domain name>\n", argv[1]);
        exit(1);
    }    

    struct hostent* host = gethostbyname(argv[1]);
    if (!host) {
        error_handling("error: gethostbyname");
    }

    printf("Official domain name: %s\n", host->h_name);
    for (int i = 0; host->h_aliases[i]; ++i) {
        printf("Alias %d: %s\n", i + 1, host->h_aliases[i]);
    }
    printf("Type: %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
    for (int i = 0; host->h_addr_list[i]; ++i) {
        printf("IP addr %d: %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
    return 0;
}

void error_handling(const char error[])
{
    fputs(error, stderr);
    fputc('\n', stderr);
    exit(1);
}