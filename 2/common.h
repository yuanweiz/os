#ifndef __COMMON_H
#define __COMMON_H
#define MEM_SIZE 1024*1024
#define SHM_KEY 9961 // magic number
#include <malloc.h>
#include <sys/shm.h>
#include <semaphore.h>
typedef struct shared_mem_s{
    sem_t readable, writable;
    unsigned int size;
    int shmid;
    char data[1]; //variable length field
} shared_mem;
static inline shared_mem * get_shared_mem(int create){
    const char* error_msg=NULL;
    shared_mem * ptr;
    do {
        if (create){
            create = IPC_CREAT;
        }
        int shmid= shmget( SHM_KEY, MEM_SIZE, create | 0666);
        if (shmid<0){
            error_msg = "shmget()";break;
        }
        ptr = (shared_mem*) shmat(shmid,0,0);
        ptr->shmid = shmid;
        if (ptr==(void*)-1){
            error_msg = "shmat()";break;
        }
        ptr->size = MEM_SIZE-sizeof(shared_mem);
        if (create){
            int err;
            err = sem_init(&ptr->writable,1,1);
            if (err<0){
                error_msg="sem_init(writable)";break;
            }
            err = sem_init(&ptr->readable,1,1);
            if (err<0){
                error_msg="sem_init(readable)";break;
            }
        }
        return ptr;
    }while(0);

    //failed
    printf("failed to init shared memory, error_msg=%s\n", error_msg);
    return NULL;
}

static inline void destroy_shared_mem(shared_mem * mem){
    shmctl(mem->shmid, IPC_RMID, 0);
    //sem_destroy(&mem->readable);
    //sem_destroy(&mem->writable);
}

#endif// __COMMON_H
