#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

struct cmd{
    int argc;
    char *argv[4];
};
struct cmd cmd[]={
    {2,{"cat","/etc/passwd",NULL}},
    {2,{"grep","root",NULL}},
    {2,{"wc","-l",NULL}}
};
void work(int argc){
    if(argc < 0)return;
    int fd[2];
    pipe(fd);
    int pid = fork();
    if(pid == 0){
        if(argc > 0){
            dup2(fd[1],1);
            close(fd[0]);
            close(fd[1]);
        }
        work(argc-1);
        exit(0);
    }else{
        if(argc > 0){
            dup2(fd[0],0);
            close(fd[0]);
            close(fd[1]);
        }
        execvp(cmd[argc].argv[0],cmd[argc].argv);
    }
}
int main(){
    work(2);
};
