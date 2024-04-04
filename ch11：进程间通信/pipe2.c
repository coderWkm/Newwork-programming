#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define BUF_SIZE 30

/// @brief 修改pipe2喂pipe3.c
/// @param argc 
/// @param argv 
/// @return 
int main(int argc, char* argv[])
{
    char buf[BUF_SIZE];
    char str1[] = "Info1 from child writes.";
    char str2[] = "Info2 from parent writes.";
    int fds1[2], fds2[2];

    pipe(fds1), pipe(fds2);
    __pid_t pid = fork();
    if (!pid) {
        write(fds1[1], str1, sizeof(str1));
        //sleep(2);
        read(fds2[0], buf, sizeof(buf));
        printf("Child get info: %s\n", buf);
    }
    else {
        read(fds1[0], buf, sizeof(buf));
        printf("Parent get info: %s\n", buf);
        write(fds2[1], str2, sizeof(str2));
        //sleep(3);
    }

    return 0;
}