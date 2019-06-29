#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/types.h>
#define LEN 256
#define WIDTH 256
#define HEIGHT 10
void split(char source[],char dest[HEIGHT][WIDTH]){
    char *p;
    p=strtok(source," ");
    int i=0;
    for(i=0;p[i]!='\0';i++){
        dest[0][i]=p[i];
    }
    dest[0][i]='\0';
    int j=1;
    while(p){
        p=strtok(NULL," ");
        if(p){
            for(i=0;p[i]!='\0';i++){
                dest[j][i]=p[i];
            }
            dest[j][i]='\0';
            j++;
        }
    }
}
void mysys(char comm[HEIGHT][WIDTH]){
    char *p[256];
    int i;
    for(i=0;i<HEIGHT;i++){
        if(strcmp(comm[i],"\0")!=0){
            p[i]=comm[i];
        }else{
            break;
        }
    }
    p[i]=NULL;
    int pid=fork();
    if(pid==0){
        if(execvp(p[0],p)<0){
            printf("Error in child\n");
        }
        exit(123);
    }
    int status;
    wait(&status);
    if(WIFEXITED(status)){
        return;
    }
}
void excute(char comm[HEIGHT][WIDTH]){
    if(strcmp(comm[0],"echo")==0){
        int i=0,is=0;
        for(i=0;comm[i][0]!='\0';i++){
            if(comm[i][0]=='>'){
                is=1;break;
            }
        }
        if(is==1){
            int fd;
            fd=open(&comm[i][1],O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
            //dup2(fd,1);
            //close(fd);
            write(fd,comm[i-1],strlen(comm[i-1]));
            close(fd);
        }else{
            mysys(comm);
        }
    }else if(strcmp(comm[0],"exit")==0){
        exit(0);
    }else if(strcmp(comm[0],"cd")==0){
        if(chdir(comm[1])){
            printf("command cd:no such file or dir:%s\n",comm[1]);
        }
    }else if(strcmp(comm[0],"cat")==0&&strcmp(comm[3],"wc")==0){
        //printf("my cat | wc\n");
        int pid,fd[2];
        char buf[32];
        pipe(fd);
        pid=fork();
        if(pid==0){
           dup2(fd[1],1);
           close(fd[0]);
           close(fd[1]);
           if(execlp("cat","cat",comm[1],NULL)<0){
               printf("Error In Child!\n");
           }
           exit(123);
        }
        int status;
        wait(&status);
        if(WIFEXITED(status)){
            dup2(fd[0],0);
            close(fd[0]);
            close(fd[1]);
            if(execlp("wc","wc",comm[4],NULL)<0){
                printf("Error In Parent!\n");
            }
            //read(0,buf,sizeof(buf));
            //printf("Result:%s\n",buf);
        }
    }else{
        mysys(comm);
    }
}
int main(){
    int f=1;
    while(f!=0){
        char command[LEN];
        fgets(command,LEN,stdin);
        command[strlen(command)-1]='\0';
        if(strcmp(command,"\0")==0)continue;
        char mySplit[HEIGHT][WIDTH]={{'\0'}};
        split(command,mySplit);
        /*int j=0;
        for(j=0;j<HEIGHT;j++){
            if(strcmp(mySplit[j],"\0")==0)break;
            printf("command:%d:%s:\n",j,mySplit[j]);
        }*/
        excute(mySplit);
    }
    return 0;
}