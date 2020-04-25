
#include "rcp.h"

void rcp::conn_recv(struct sockaddr_in* client, int ttl){
    /**
     * @brief: resets the time to live for a specified client
     * @param client: the client is question
     * @param ttl: the time to live for the client
    */
    //critical section where we will be writing to map in conn_rev
    // and check_conn
    pthread_mutex_lock(&clmtx);
    clients[client] = ttl;
    pthread_mutex_unlock(&clmtx);

}


void rcp::conn_send(int sock, int wait, struct sockaddr_in addr){
    /**
     * @brief: continually pings server to let it know it is still 
     * alive and connected
     * @param sock: the socket handle to send the data on
     * @param wait: time (seconds) between sending conn requests
     * @param addr: the socket address to send on
    */
    
    const char* conn_msg = "syn";
    for(;;){
        sendto(sock, conn_msg, sizeof(conn_msg),0,(struct sockaddr*)&addr,(socklen_t)sizeof(addr));
        sleep(wait);
    }
}





void rcp::check_conn(int wait){
    /**
     * @brief: iterates over the client list and decrements
     * each client's time to live (ttl). if a client's ttl is 0,
     * it is removed
     * @param wait: time (seconds) the time to wait for each iteration
     * over client list.
    */
    for(;;){
    
        pthread_mutex_lock(&clmtx);
        for(map<struct sockaddr_in*,int>::iterator iter = clients.begin(); 
        (iter != clients.end()) && (clients.size()>0); ++iter){
            
            struct sockaddr_in* cl = iter->first;
            cout<<clients.size()<<endl;
            clients[cl]--;
            //client inactive, remove from list
            if(clients[cl] <= 0){
                clients.erase(cl);
                cout<<"client disconnect"<<endl;
            }
        }
        pthread_mutex_unlock(&clmtx);
        sleep(wait);
    }
}


void rcp::recv_msg (int sock, struct sockaddr_in this_addr){
    /**
     * @brief: receives and handles any message sent from clients
     * @param sock: the socket handle to use
     * @param this_addr: the address struct of this machine
    */
    struct sockaddr_in client;
    memset(&client,0,sizeof(client));


    bind(sock,(struct sockaddr*)&this_addr, sizeof(this_addr));
    char msg[1024];

    for(;;){
        recvfrom(sock, msg,1024,MSG_CONFIRM,(struct sockaddr*)&client,(socklen_t*)sizeof(client));
        cout<<(const char*)msg<<endl;
        if(!strcmp("syn",(const char*)msg)){//if strings are equal
            
            thread conn_check(&rcp::conn_recv,this,&client,5);
            conn_check.detach();
            
        }
    }
}


void rcp::rcp_listen(int port, bool block = 0){
    /**
     * @brief: binds to this machine address and sets up
     * reception of client messages
     * @param port: the port to listen on
     * @param block: True for blocking, False for asychronous mode
    */
    
    struct sockaddr_in this_addr;
    memset(&this_addr,0,sizeof(this_addr));
    this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this_addr.sin_port = htons(port);
    this_addr.sin_family = AF_INET;

    thread ch_conn(&rcp::check_conn,this,1);
    ch_conn.detach();

    thread recv_msg_thread(&rcp::recv_msg,this,sock,this_addr);
    if(block){
        recv_msg_thread.join();
    }else{
        recv_msg_thread.detach();
    }
}

rcp::rcp(){
    sock = socket(AF_INET, SOCK_DGRAM,0);
}

rcp::~rcp(){}


void rcp::connect(const char* ip, int port, bool block = 0){
    /**
     * @brief: continuosly sends connection requests to server
     * @param ip: the ip address of the server
     * @param port: the port of the server
     * @param block: True for blocking mode, False for asychronous mode
    */
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    thread conn_send_thread(&rcp::conn_send,this,sock,1,addr);
    if(block){
        conn_send_thread.join();    
    }else{
        conn_send_thread.detach();
    }
    
}


