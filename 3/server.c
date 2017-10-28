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

int start_server(const char *ipstr, uint16_t port){
    int err;
    int listenfd = -1;
    do{
        listenfd=socket(AF_INET, SOCK_STREAM, 0);
        if(listenfd<0)break;
        struct sockaddr_in local_addr;
        fill_ip_addr(&local_addr, ipstr, port);
        err = bind(listenfd, (const struct sockaddr*)&local_addr,sizeof(local_addr));
        if (err<0)break;
        err = listen(listenfd,0);
        if (err<0)break;
        return listenfd;
    }while(0);

    if(listenfd>0) 
        close(listenfd); //do clean-up jobs
    return -1;
}

char buf[1024];
void loop(int peerfd){
    char buf[1024];
    int sz;
    while(1){
        uint32_t msg_len;
        sz=read(peerfd,&msg_len,sizeof(msg_len));
        if(sz< 4 )break;
        if (msg_len > sizeof(buf)-1){
            puts("insane: message too long");
            break;
        }
        sz=read(peerfd,buf,msg_len);
        if (sz <0 || sz< (int)msg_len)break;
        buf[sz]='\0';
        printf("%s",buf);
    };
}
int main (){
    int listenfd=start_server("0.0.0.0",8765);
    if (listenfd <0){
        puts("error occured on start_server()");
        return listenfd;
    }
    int peer=accept(listenfd, NULL,NULL);
    if (peer<0){
        puts("error occured on accept()");
        return peer;
    }
    loop(peer);
    return 0;
}
