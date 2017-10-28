#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <inttypes.h>

void fill_ip_addr(struct sockaddr_in* addr, const char * ipstr, uint16_t port){
    bzero(addr, sizeof(struct sockaddr_in));
    addr->sin_port=htons(port);
    addr->sin_family = AF_INET;
    inet_aton(ipstr, &addr->sin_addr);
}

int connect_server(const char *ipstr, uint16_t port){
    int connfd=-1;
    int err;
    do{
        connfd=socket(AF_INET, SOCK_STREAM, 0);
        if(connfd<0)break;
        struct sockaddr_in local_addr;
        fill_ip_addr(&local_addr, "0.0.0.0", 0);
        err = bind(connfd, (const struct sockaddr*)&local_addr,sizeof(local_addr));
        if (err<0)break;
        struct sockaddr_in peer_addr;
        fill_ip_addr(&peer_addr, ipstr, port);
        err = connect(connfd, (const struct sockaddr*) &peer_addr, sizeof(peer_addr)); 
        if (err <0)break;
        return connfd;
    }while(0);

    if(connfd>0) 
        close(connfd); //do clean-up jobs
    return -1;
}

char buf[1024];
void loop(int connfd){
    while(1){
        char * ret=fgets(buf,sizeof(buf),stdin);
        if (!ret)return;
        uint32_t sz=strlen(buf);
        int err;
        err = write(connfd,&sz,sizeof(sz));
        if(err<0)break;
        err = write(connfd,buf,sz);
        if(err<0)break;
    }
}
int main (){
    int connfd=connect_server("127.0.0.1",8765);
    if (connfd <0){
        puts("error occured on setup()");
        return connfd;
    }
    loop(connfd);
    return 0;
}
