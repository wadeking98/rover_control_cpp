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
#define BUFF_SIZE 1024
#define PORT 5555
#define PING_WAIT 1
using namespace std;

//we get a seg fault if we try and lock a pointer
class rcp{
    private:
        int sock;
        map<struct sockaddr_in*, int> clients;
        
        pthread_mutex_t clmtx;
        void conn_recv(struct sockaddr_in*, int);
        void recv_msg(int, struct sockaddr_in);
        void check_conn(int);
        void conn_send(int, int, struct sockaddr_in);
        
    public:
        rcp();
        ~rcp();
        void init();
        void connect(const char*, int, int);
        void rcp_listen(int, int);

};





#endif
