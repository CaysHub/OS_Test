#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(int argc,char *argv[]){
    printf("file:%s\n",argv[1]);
    int of=open(argv[1],O_RDWR);
    char buf[1025];
    int flag=-1;
    while(1){
        flag=read(of,buf,1024);
	    if(flag<=0)break;
	    buf[flag]=0;
	    printf("%s",buf);
    }
    close(of);
    return 0;
}
