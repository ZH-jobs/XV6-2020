#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define WR 1
#define RD 0

const uint INT_LEN=sizeof(int);

int First_number(int p[2],int *first){//用于获取首数字并打印出来
    if(read(p[RD],first,INT_LEN)==INT_LEN){
        printf("prime %d\n",*first);
        return 0;
    }
    return -1;
}

void Extract_the_remaining_numbers(int left_pipe[2],int right_pipe[2],int first){//从左边获取数字并且进行筛选
    int number;

    while(read(left_pipe[RD],&number,sizeof(int))==sizeof(int)){

        if(number%first){
            write(right_pipe[WR],&number,sizeof(int));
        }
    }

    close(left_pipe[RD]);
    close(right_pipe[WR]);
}

void primes(int left_pipe[2]){
    int first;
    close(left_pipe[WR]);
    if(First_number(left_pipe,&first)==0){
        int right_pipe[2];
        pipe(right_pipe);

        Extract_the_remaining_numbers(left_pipe,right_pipe,first);

        if(fork()==0){
            primes(right_pipe);
        }else{
            close(right_pipe[RD]);
            wait(0);
        }
    }
    exit(0);
}

int
main(int argc,char *argv[]){
    int p[2];
    pipe(p);
    for(int i=2;i<=35;++i){
        write(p[WR],&i,INT_LEN);
    }
    if (fork() == 0) {
        primes(p);
    } else {
        close(p[WR]);
        close(p[RD]);
        wait(0);
    }
    exit(0);
}

