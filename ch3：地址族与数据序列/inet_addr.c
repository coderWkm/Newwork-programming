#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    char* inet1 = "1.2.3.4";
    char* inet2 = "1.2.3.256";

    unsigned long conv_inet1 = inet_addr(inet1);
    if (conv_inet1 == INADDR_NONE) {
        printf("Error occured\n");
    } 
    else {
        printf("Network ordered integer addr: %#lx \n", conv_inet1);
    }

    conv_inet1 = inet_addr(inet2);
    if (conv_inet1 == INADDR_NONE) {
        printf("Error occured\n");
    } 
    else {
        printf("Network ordered integer addr: %#lx \n", conv_inet1);
    }

    return 0;
}