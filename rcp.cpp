
#include "rcp.h"
int sock;
map<struct sockaddr_in*, int> clients;
mutex clmtx;

void conn_recv(struct sockaddr_in* client, int ttl){

    //critical section where we will be writing to map in conn_rev
    // and check_conn
    clmtx.lock();
    clients[client] = ttl;
    clmtx.unlock();

}


void conn_send(int sock, int wait, struct sockaddr_in addr){
    /**
     * @brief: continually pings server to let it know it is still 
     * alive and connected
     * @param args: stuct containing thread args: (struct sockaddr_in* addr, agent* a, int wait)
     * addr: the server address structure
     * a: the connection agent
     * wait: the amount of time to wait between pings
    */
    
    const char* conn_msg = "syn";
    for(;;){
        sendto(sock, conn_msg, sizeof(conn_msg),0,(struct sockaddr*)&addr,(socklen_t)sizeof(addr));
        sleep(wait);
    }
}





void check_conn(int wait){
    for(;;){
        clmtx.lock();
        for(map<struct sockaddr_in*,int>::iterator iter = clients.begin(); iter != clients.end(); ++iter){
            struct sockaddr_in* cl = iter->first;
            clients[cl]--;
            //client inactive, remove from list
            if(clients[cl] <= 0){
                clients.erase(cl);
            }
        }
        clmtx.unlock();
        sleep(wait);
    }
}


void recv_msg (int sock, struct sockaddr_in this_addr){
    struct sockaddr_in client;
    memset(&client,0,sizeof(client));


    bind(sock,(struct sockaddr*)&this_addr, sizeof(this_addr));
    char msg[1024];

    for(;;){
        int n = recvfrom(sock, msg,1024,MSG_CONFIRM,(struct sockaddr*)&client,(socklen_t*)sizeof(client));
        cout<<(const char*)msg<<endl;
        if(!strcmp("syn",(const char*)msg)){//if strings are equal
            
            thread conn_check(conn_recv,&client,5);
            conn_check.detach();
            
        }
    }
}


void rcp_listen(int port, int block){
    
    struct sockaddr_in this_addr;
    memset(&this_addr,0,sizeof(this_addr));
    this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this_addr.sin_port = htons(port);
    this_addr.sin_family = AF_INET;

    thread ch_conn(check_conn,5);
    ch_conn.detach();

    thread recv_msg_thread(recv_msg,sock,this_addr);
    if(block){
        recv_msg_thread.join();
    }else{
        recv_msg_thread.detach();
    }
}

void init(){
    sock = socket(AF_INET, SOCK_DGRAM,0);
}

void connect(const char* ip, int port, int block){
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    thread conn_send_thread(conn_send,sock,1,addr);
    if(block){
        conn_send_thread.join();    
    }else{
        conn_send_thread.detach();
    }
    
}


