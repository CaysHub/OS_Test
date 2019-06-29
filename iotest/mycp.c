#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc,char *argv[]){
    int of=open(argv[1],O_RDWR);
    int out=open(argv[2],O_RDWR|O_CREAT);
    char buf[1025];
    int flag=1;
    while(1){
        flag=read(of,buf,1024);
	    if(flag==0)break;
	    buf[flag]='\0';
	    int success=write(out,buf,flag);
    }
    close(of);
    close(out);
    return 0;
}
