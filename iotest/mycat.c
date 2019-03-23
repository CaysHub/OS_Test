#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main(){
    int of=open("passwd.bat",O_RDWR);
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
