#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
struct param{
    double start,end;
};
struct result{
    double r;
};
void *worker(void *arg){
    struct result *result;
    result=malloc(sizeof(struct result));
    result->r=0;
    int i;
    for(i=1;i<=50;i++){
        result->r+=i;
    }
    return result;
}
double master(){
    int i;
    double sum=0;
    for(i=51;i<=100;i++){
        sum+=i;
    }
    return sum;
}
int main(int argc,char *argv[]){
    pthread_t worker_tid;
    double sum=0,pi=0;
    pthread_create(&worker_tid,NULL,worker,NULL);
    pi=master();
    struct result *result;
    pthread_join(worker_tid,(void **)&result);
    printf("result = %lf\n",result->r);
    pi+=result->r;
    printf("sum = %lf\n",pi);
    return 0;
}
