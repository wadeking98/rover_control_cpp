#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <array>
#include <pthread.h>
#include <unistd.h>
#define PING_WAIT 1
#define PORT 5555
using namespace std;

struct conn_send_args{
    struct sockaddr_in addr;
    int sock;
    int wait;
};

void * conn_send(void * args){
    /**
     * @brief: continually pings server to let it know it is still 
     * alive and connected
     * @param args: stuct containing thread args: (struct sockaddr_in* addr, agent* a, int wait)
     * addr: the server address structure
     * a: the connection agent
     * wait: the amount of time to wait between pings
    */
    struct sockaddr_in addr = ((struct conn_send_args*)args)->addr;
    int sock = ((struct conn_send_args*)args)->sock;
    int wait = ((struct conn_send_args*)args)->wait;
    const char* conn_msg = "syn";
    for(;;){
        sendto(sock, conn_msg, sizeof(conn_msg),0,(struct sockaddr*)&addr,(socklen_t)sizeof(addr));
        sleep(wait);
    }
}

int main(int argc, char** argv){
    if(argc < 2){
        cout<<"Usage: bash controller <rover_ip>"<<endl;
    }
    // agent a;

    // a.agent_send("test",argv[1],PORT);
    int sock = socket(AF_INET, SOCK_DGRAM,0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    struct conn_send_args cn_args;
    cn_args.sock = sock;
    cn_args.addr = addr;
    cn_args.wait = PING_WAIT;

    pthread_t conn;
    pthread_create(&conn, NULL, conn_send,&cn_args);
    pthread_join(conn,NULL);
}