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
#include <pthread.h>
#include <mutex>
#define BUFF_SIZE 1024
#define PORT 5555
#define PING_WAIT 1
using namespace std;

//we get a seg fault if we try and lock a pointer
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
        void send(int, void* , struct sockaddr_in);
        
        
    public:
        rcp();
        ~rcp();
        void connect(const char*, int, bool);
        void serve(int);
        void listen(bool, bool);
        void send(void*, const char*, int);
        

};

#endif

