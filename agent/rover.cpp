#include "rcp.h"

int main(){
    rcp rcp_conn;
    
    rcp_conn.serve(5555);
    rcp_conn.listen(1024, true, false);

    for(;;){
        printf("%s\n", rcp_conn.read());
        sleep(1);
    }
}