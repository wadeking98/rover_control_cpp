
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


void rcp::send(int sock, void* msg, struct sockaddr_in addr){
    /**
     * @brief: private send method, sends a message to addr struct
     * @param sock: the socket handle to send on
     * @param msg: the content to send
     * @param addr: the sockaddr_in struct to send to
    */
    sendto(sock, msg, sizeof(msg), 0, (struct sockaddr*)&addr,(socklen_t)sizeof(addr));
}

void rcp::send(void* msg, const char* ip, int port){
    /**
     * @brief: public send method, sends a message to specified ip, port
     * @param msg: the content to send
     * @param ip: the ip address to send to
     * @param port: the port to send to
    */
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    send(sock,msg,addr);
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
        //iterate over map
        map<struct sockaddr_in*,int>::iterator iter;
        for( iter = clients.begin(); (iter != clients.end()) && (clients.size()>0); ++iter){
            
            struct sockaddr_in* cl = iter->first;
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


void rcp::recv_msg (int sock, int buffsize = 1024, bool conn = false){
    /**
     * @brief: receives and handles any message sent from clients
     * @param sock: the socket handle to use
     * @param conn: true if connections are to be accepted
    */
    struct sockaddr_in client;
    memset(&client,0,sizeof(client));


    char msg[buffsize];

    for(;;){
        socklen_t len;
        recvfrom(sock, msg,buffsize,MSG_WAITALL,(struct sockaddr*)&client,&len);
        cout<<(const char*)msg<<endl;
        if(!strcmp("syn",(const char*)msg) && conn){//if strings are equal and connections are accepted
            thread conn_check(&rcp::conn_recv,this,&client,5);
            conn_check.detach();
            send(sock,(void*)"ack",client);
        }
    }
}


void rcp::serve(int port){
    /**
     * @brief: binds to this machine address and sets up
     * reception of client messages
     * @param port: the port to listen on
    */
    
    struct sockaddr_in this_addr;
    memset(&this_addr,0,sizeof(this_addr));
    this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    this_addr.sin_port = htons(port);
    this_addr.sin_family = AF_INET;

    bind(sock,(struct sockaddr*)&this_addr, sizeof(this_addr));

    
}

void rcp::listen(bool conn = false, bool block = false){
    /**
     * @brief: listen for incoming messages
     * @param conn: true to listen for connections
     * @param block: true from blocking mode, false for asych mode
    */
    if(conn){//if connections are accepted
        thread ch_conn(&rcp::check_conn,this,1);
        ch_conn.detach();
    }
    
    thread recv_msg_thread(&rcp::recv_msg,this,sock,conn);
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


