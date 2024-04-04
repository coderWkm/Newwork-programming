#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


void timeout(int sig) {
    int status;
    __pid_t pid = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Child proc id: %d\n", pid);
        printf("Child send: %d\n", WEXITSTATUS(status));
    }
}

int main(int argc, char *argv[])
{
    pid_t pid;
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid = fork();
    if (!pid) {
        printf("The 1st child process");
        sleep(10);
        return 12;
    }
    else {
        printf("1st child process id: %d\n", pid);
        pid = fork();
        if (!pid) {
            puts("The 2nd child proc");
            sleep(10);
            return 24;
        }
        else {
            printf("2nd process id: %d\n", pid);
            for (int i = 0; i < 3; ++i) {
                puts("wait...");
                sleep(5);
            }
        }
    }

    return 0;
}