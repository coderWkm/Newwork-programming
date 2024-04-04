#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    __pid_t pid = fork();
    if (!pid) {
        puts("This is a child process.");
    }
    else {
        printf("Child process ID: %d\n", pid);
        sleep(10);
    }

    if (!pid) {
        puts("End child process");
    }
    else {
        puts("End parent process");
    }
    return 0;
}