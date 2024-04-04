#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int status;
    __pid_t pid = fork();

    if (!pid) {
        return 3;
    }
    else {
        printf("Child PID: %d\n", pid);
        pid = fork();
        if (pid == 0) {
            exit(8);
        }
        else {
            printf("Child PID: %d\n", pid);
            wait(&status);
            if (WIFEXITED(status)) {
                printf("child send one: %d\n", WEXITSTATUS(status));
            }

            wait(&status);
            if (WIFEXITED(status)) {
                printf("child send one: %d\n", WEXITSTATUS(status));
            }
            sleep(10); 
        }
    }

    return 0;
}