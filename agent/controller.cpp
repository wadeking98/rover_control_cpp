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
        exit(1);
    }
    rcp rcp_conn;
    rcp_conn.listen(1024, false, false);
    rcp_conn.connect(argv[1],5555, false);

    for(;;){
        sleep(1);
        rcp_conn.send("qwerty", argv[1], 5555);
    }
    
}