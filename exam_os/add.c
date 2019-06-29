#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
    int pid;
    int fd[2];

    pipe(fd);
    pid = fork();
    if(pid == 0){
        int i,sum=0;
        for(i=1;i<=50;i++){
            sum+=i;
        }
        close(fd[0]);
        write(fd[1],(void *)&sum,sizeof(int));
        exit(0);
    }
    close(fd[1]);
    int sum1=0,sum2=0,j;
    for(j=51;j<=100;j++){
        sum2+=j;
    }
    read(fd[0],(void *)&sum1,sizeof(int));
    printf("sum = %d\n",sum1+sum2);
    return 0;
}
