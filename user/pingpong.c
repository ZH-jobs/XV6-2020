#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define WR 1
#define RD 0

int 
main(int argc,char *argv[]){
    int p1[2];
    int p2[2];
    pipe(p1);//parent -> child 
    pipe(p2);//child ->  parent
    char buf = 'p';
    int ResStatus =0;
    int pid=fork();
    if(pid<0){
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
    }else if(pid==0){//child
        close(p1[WR]);
        close(p2[RD]);
        if(read(p1[RD],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2, "child read() error!\n");
            ResStatus = 1;
        }else{
            fprintf(1,"%d: received ping\n",getpid());
        }
        if(write(p2[WR],&buf,sizeof(buf))!=sizeof(char)){
            fprintf(2, "child write() error!\n");
            ResStatus = 1;
        }
        close(p1[RD]);
        close(p2[WR]);
    }else{//parent
        close(p1[RD]);
        close(p2[WR]);
        if(write(p1[WR],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2, "parent write() error!\n");
        }

        if(read(p2[RD],&buf,sizeof(char))!=sizeof(char)){
            fprintf(2, "parent read() error!\n");
            ResStatus = 1;
        }else{
            fprintf(1,"%d: received ping\n",getpid());
        }
        close(p1[WR]);
        close(p2[RD]);
    }
    
    exit(ResStatus);
}




