#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{   
    unsigned short host_port = 0x1234;
    unsigned short net_port;
    unsigned long host_ip = 0x12345678;
    unsigned long net_ip;

    net_port = htons(host_port);
    net_ip = htonl(host_ip);

    printf("Host ordered port: %#x \n", host_port);
    printf("Network ordered port: %#x \n", net_port);
    printf("Host ordered ip: %#lx \n", host_ip);
    printf("Newwork ordered ip: %#lx \n", net_ip);


    return 0;
}