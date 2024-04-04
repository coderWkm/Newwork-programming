#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
    char buf[BUF_SIZE] = "Who are you?";
    char recv[BUF_SIZE];
    memset(recv, '\0', sizeof(recv));
    int fds[2];

    pipe(fds);
    __pid_t pid = fork();
    if (!pid) {
        write(fds[1], buf, sizeof(buf));
    }       
    else {
        read(fds[0], recv, BUF_SIZE);
        puts(recv);
    }
    return 0;
}