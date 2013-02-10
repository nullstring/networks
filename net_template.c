#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints,0,sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    //sets localhost to addr structs  -- set NULL to ipaddr in getaddrinfo() 
    //hints.ai_flags = AI_PASSIVE;

    if((status = getaddrinfo("202.141.80.14","80",&hints,&servinfo)) != 0 ){
        fprintf(stderr,"getaddrinfo error : %s \n",gai_strerror(status));
        exit(1);
    } 

    
    //stuff


    freeaddrinfo(servinfo);
    return 0;

}
