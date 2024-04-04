#include <unistd.h>
#include <stdio.h>

int gval = 10;
int main(int argc, char* argv[])
{
    int lval = 15;
    ++gval; lval += 5;
    printf("gval: %d, lval: %d\n", gval, lval);
    int child = fork();
    if (!child) {
        ++gval;
        ++lval;
        puts("Child process!");
    }
    else {
        --gval;
        --lval;
        puts("Main process!");
    }
    printf("After fork, gval: %d, lval: %d\n", gval, lval);
    return 0;
}