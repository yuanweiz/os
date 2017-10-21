#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

shared_mem * mem;
FILE* outfile;
void cleanup();
void onSigInt(int);
void setup(){
    mem = get_shared_mem(0); //do not create new mem
    if (!mem){
        puts("you should start receiver first");
        exit(1);
    }
    outfile=fopen("secrets.out","w");
    //outfile = stdout;
    atexit(&cleanup);
    signal (SIGINT, &onSigInt);
    signal (SIGTERM, &onSigInt);
}

void loop(){
    while(1){
        sem_wait(&mem->readable);
        //printf("%s",mem->data);
        const char* probe=mem->data;
        int cnt=0;
        while(*probe){
            if (*probe>='0' && *probe<='9')cnt++;
            ++probe;
        }
        fprintf(outfile, "%d %s",cnt, mem->data);
        sem_post(&mem->writable);
    }
}

void onSigInt(int sig){
    (void) sig;
    exit(0);
}
void cleanup(){
    puts("cleaning up...");
    if (mem)
        destroy_shared_mem(mem);
    fclose(outfile);
}

int main (){
    setup();
    loop();
    return 0;
}
