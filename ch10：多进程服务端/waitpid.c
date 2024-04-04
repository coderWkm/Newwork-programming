#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int status;
    __pid_t pid = fork();

    if (!pid) {
        sleep(5);
        return 8;
    }
    else {
        while (!waitpid(-1, &status, WNOHANG)) {
            sleep(1);
            puts("sleep 1s");
        }
        if (WIFEXITED(status)) {
            printf("child send %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}