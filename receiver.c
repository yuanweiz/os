#include "common.h"
#include <stdio.h>
shared_mem* mem;

void setup(){
    mem = get_shared_mem(1); //create
}

void loop(){
    while(1){
        sem_wait(&mem->writable);
        fgets(mem->data,mem->size,stdin);
        sem_post(&mem->readable);
    }
}

int main (){
    setup();
    loop();
    //FIXME: maybe we should do some cleanup?
    return 0;
}
