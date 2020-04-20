#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "rcp.h"
int main(int argc, char** argv){
    if(argc < 2){
        printf("Usage: bash controller <rover_ip>");
    }
    init();
    connect(argv[1],5555, true);
}