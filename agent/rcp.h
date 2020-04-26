#include <sys/socket.h>
#include <string.h>
#ifndef RCP_H
#define RCP_H
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <mutex>
#define BUFF_SIZE 1024
#define PORT 5555
#define PING_WAIT 1
#define FLAG_SIZE 1
using namespace std;

//we get a seg fault if we try and lock a pointer
enum FLAG{SYN=0x01, DAT=0x02};

class rcp{
    private:
        int sock;
        map<struct sockaddr_in*, int> clients;
        void* buff;
        pthread_mutex_t clmtx;
        void conn_recv(struct sockaddr_in*, int);
        void recv_msg(int, int, bool);
        void check_conn(int);
        void conn_send(int, int, struct sockaddr_in);
        void send(int, const char* , struct sockaddr_in, FLAG);
        
        
    public:
        rcp();
        ~rcp();
        void connect(const char*, int, bool);
        void serve(int);
        void listen(int,bool, bool);
        void send(const char*, const char*, int, FLAG);
        

};

#endif

