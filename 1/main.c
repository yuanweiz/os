#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <string.h>

#define MAX_ARGNUM 1023
int isBlank(char c){
    if( c==' '|| c=='\t')return 1;
    return 0;
}
char ** parse(char* cmd, ssize_t len){
    static char* buf[MAX_ARGNUM+1];
    cmd[len-1]='\0';
    int idx=0;
    char * pch=cmd;
    while (*pch){
        while(*pch&& isBlank(*pch)){
            *pch='\0';
            ++pch;
        }
        if (!*pch)break;
        buf[idx++]=pch;
        while (*pch&& !isBlank(*pch)){
            ++pch;
        }
    }
    buf[idx]=NULL;
    return buf;
}

int main (){
    char *cmd = NULL;
    size_t bufSize = 0;
    ssize_t nread;
    while (1) {
        printf("prompt> ");
        if ((nread = getline(&cmd, &bufSize, stdin)) == -1)
            break;
        char** cmdv = parse(cmd,nread);
        if (*cmdv==NULL){
            //empty line
        }
        else {
            char program[128];
            int ret= snprintf(program,sizeof(program)-1, "/usr/bin/%s",cmdv[0]);
            program[ret]='\0';
            pid_t child = fork();
            if (child ==0 ){
                //this is the child process
                execv(program, cmdv);
                fprintf(stderr,"exec() failed\n");
                return 0;
            }
            else {
                waitpid(child,NULL,0);
            }
        }
    }
    free(cmd);
    return 0;
}
