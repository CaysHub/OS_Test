#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
void child(char *argv[]){
    int error=execvp(argv[0],argv);
    if(error<0){
        printf("error/n");
    }
    exit(123);
}
void mysys(char *command){
    char *p[256];int i=0,j;
    char *p1;
    char buf[2048];
    strcpy(buf,command);
    p1=strtok(buf," ");
    while (p1)
    {
        char *tbuf;
        tbuf=p1;
        p[i++]=tbuf;
        p1=strtok(NULL," ");
    }
    p[i]=NULL;
    int pid=fork();
    if(pid==0){
        child(p);
    }
    int status;
    wait(&status);
    if(WIFEXITED(status)){
        return;
    }
    
}
int main()
{
    printf("--------------------------------------------------\n");
    mysys("echo HELLO WORLD");
    printf("--------------------------------------------------\n");
    mysys("ls /");
    printf("--------------------------------------------------\n");
    return 0;
}
