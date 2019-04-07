#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
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
    char *p[256];int i;
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
        int error = execvp(p[0],p);
        if(error<0){
            printf("error\n");
        }
        exit(123);
    }
    int status;
    wait(&status);
    if(WIFEXITED(status)){
        return;
    }
}
int execute(char comm[HEIGHT][WIDTH]){
    if(strcmp(comm[0],"echo")==0){
        int i=0,is=0;
        for(i=0;comm[i][0]!='\0';i++){
            if(comm[i][0]=='>'){
		    is=1;break;
	    }
	}
	if(is==1){
            //printf("file name:%s\n",&comm[i][1]);
    	    int fd;
            //在当前目录下创建一个文件
	    fd = open(&comm[i][1], O_CREAT|O_RDWR, S_IRUSR|S_IWUSR); 
            //新打开的文件描述符是3
            //使用dup2将标准输出重定向到文件log
	    //文件描述符1是标准输出
	    //fd指向log文件
            dup2(fd, 1); 
            close(fd); 
	    //最终输出保存到文件log
	    //printf("context:%s length:%d\n",comm[i-1],strlen(comm[i-1]));
            write(1, comm[i-1], strlen(comm[i-1]));
	    dup2(1,1); 
	    return 1;
        }else{
            mysys(comm);
        }
    }else if(strcmp(comm[0],"exit")==0){
        return -1;
    }else{
        mysys(comm);
    }
    return 1;
}
int main(){
    while(1){
        char command[LEN];
        char splitArray[HEIGHT][WIDTH]={{'\0'}};
        //printf("%s",">>");
        gets(command);
        split(command,splitArray);
	for(int i=0;i<HEIGHT;i++){
	    if(strcmp(splitArray[i],"\0")==0)break;
	    //printf("command:%2d:%s\n",i+1,splitArray[i]);
	}
        int i=0;
        if(execute(splitArray)==-1){
            return 0;
        }
    }
}

