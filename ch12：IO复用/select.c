#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
    fd_set reads, temps;
    FD_ZERO(&reads);
    FD_SET(0, &reads);
    struct timeval timeout;
    int result;
    char buf[BUF_SIZE];

    while (1) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temps, 0, 0, &timeout);
        if (result == -1) {
            puts("error: select");
            break;
        }         
        else if (result == 0) {
            puts("Timeout: select");
        }       
        else {
            if (FD_ISSET(0, &temps)) {
                int str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = '\0';
                printf("message from console: %s\n", buf);
            }
        }
    }

    return 0;
}