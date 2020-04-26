#include "rcp.h"

int main(){
    rcp rcp_conn;
    
    rcp_conn.serve(5555);
    rcp_conn.listen(1024, true, true);
}