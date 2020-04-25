#include "rcp.h"

int main(){
    rcp rcp_conn;
    rcp_conn.init();
    rcp_conn.rcp_listen(5555,true);
}