#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <array>
#include <pthread.h>
#include <map>
#include <unistd.h>
#define BUFF_SIZE 1024
#define PORT 5555
using namespace std;

//we get a seg fault if we try and lock a pointer
pthread_mutex_t clmtx;
map<struct sockaddr_in*, int> clients;

struct conn_recv_args
{
    struct sockaddr_in* client;
    int ttl;
};


void * conn_recv(void * args){
    int ttl = ((struct conn_recv_args*)args)->ttl;
    struct sockaddr_in *client = ((struct conn_recv_args*)args)->client;

    //critical section where we will be writing to map in conn_rev
    // and check_conn
    pthread_mutex_lock(&clmtx);
    clients[client] = ttl;
    pthread_mutex_unlock(&clmtx);
}

struct check_conn_args{
    int wait;
};

void * check_conn(void * args){
    int wait = ((struct check_conn_args*)args)->wait;
    for(;;){
        pthread_mutex_lock(&clmtx);
        for(map<struct sockaddr_in*,int>::iterator iter = clients.begin(); iter != clients.end(); ++iter){
            struct sockaddr_in* cl = iter->first;
            clients[cl]--;
            //client inactive, remove from list
            if(clients[cl] <= 0){
                clients.erase(cl);
            }
        }
        pthread_mutex_unlock(&clmtx);
        sleep(wait);
    }
}

struct listen_args{
    int sock;
    struct sockaddr_in this_addr;
};


void * listen (void * args){
    int sock = ((struct listen_args*)args)->sock;
    struct sockaddr_in this_addr = ((struct listen_args*)args)->this_addr;

    struct sockaddr_in client;  
    
    bind(sock,(struct sockaddr*)&this_addr, sizeof(this_addr));
    char msg[10];

    for(;;){
        int n = recvfrom(sock, msg,1024,0,(struct sockaddr*)&client,(socklen_t*)sizeof(client));
        if(!strcmp("syn",(const char*)msg)){//if strings are equal
            pthread_t conn_check;
            //create thread parameters
            struct conn_recv_args args;
            args.client = &client;
            args.ttl = 5;
            //create and detach the thread
            pthread_create(&conn_check,NULL,conn_recv, &args);
            pthread_detach(conn_check);
        }
    }

}


int main(){

    int sock = socket(AF_INET, SOCK_DGRAM,0);

    struct sockaddr_in this_addr;
    memset(&this_addr,0,sizeof(this_addr));
    this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this_addr.sin_port = htons(PORT);
    this_addr.sin_family = AF_INET;

    struct check_conn_args ch_args;
    ch_args.wait=1;


    struct listen_args l_args;
    l_args.sock = sock;
    l_args.this_addr = this_addr;

    pthread_t listener;
    pthread_t check;
    pthread_create(&check, NULL, check_conn, &ch_args);
    pthread_create(&listener, NULL,listen, &l_args);
    pthread_join(listener,NULL);


    // struct sockaddr_in clbuff;
    // void * msg = a.agent_recv(1024, &clbuff);
    // cout<<(const char*)msg<<endl;

    //int sock = socket(AF_INET, SOCK_DGRAM, 0);
}