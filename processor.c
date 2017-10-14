#include "common.h"
#include <stdio.h>
#include <stdlib.h>

shared_mem * mem;
void setup(){
    mem = get_shared_mem(0); //do not create new mem
}

void loop(){
    while(1){
        sem_wait(&mem->readable);
        printf("%s",mem->data);
        sem_post(&mem->writable);
    }
}

int main (){
    setup();
    loop();
    return 0;
}
