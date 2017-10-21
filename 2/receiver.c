#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
shared_mem* mem;

void cleanup();
void onSigInt(int);
void setup(){
    mem = get_shared_mem(1); //create
    atexit(&cleanup);
    signal(SIGINT, &onSigInt);
    signal(SIGTERM, &onSigInt);
}

void loop(){
    while(1){
        sem_wait(&mem->writable);
        while (1){
            if (fgets(mem->data,mem->size,stdin)==NULL)
                return;
            if (!(*mem->data))break;
            //puts("here");
            if (strstr(mem->data,"C00L")) 
                break;
        }
        //puts("post");
        sem_post(&mem->readable);
    }
}

void cleanup(){
    puts("cleaning up...");
    if (mem)
        destroy_shared_mem(mem);
}

void onSigInt(int sig){
    (void)sig;
    exit(0);
}
int main (){
    setup();
    loop();
    //FIXME: maybe we should do some cleanup?
    return 0;
}
