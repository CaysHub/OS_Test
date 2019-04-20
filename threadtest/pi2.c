#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#define N_CPU 8
#define N 200000
#define N_CHILD (N/N_CPU)
struct param {
    int start;
    int end;
};
struct result {
    double sum;
};
void *compute(void *arg){
    struct param *param;
    struct result *result;
    double sum = 0;
    int i;
    
    param = (struct param *)arg;
    for(i = 2 * param->start-1; i < 2 * param->end-1; i += 2){
        if(i <= 0)continue;
        sum += pow(-1 , (i-1) / 2) * 1.0/(i*1.0);
    }
    result=malloc(sizeof(struct result));
    result->sum = sum;
    return result;
}
int main(){
    pthread_t workers[N_CPU];
    struct param params[N_CPU];
    int i;
    for(i = 0; i < N_CPU; i++){
        struct param *param;
        param = &params[i];
        param->start = i * N_CHILD;
        param->end = (i + 1) * N_CHILD;
        pthread_create(&workers[i],NULL,compute,param);
    }

    double sum = 0;
    for(i = 0; i < N_CPU; i++){
        struct result *result;
        pthread_join(workers[i],(void **)&result);
        sum += result->sum;
        free(result);
    }
    double pi = 4*sum;
    printf("CYS PI = %lf\n",pi);
    return 0;
}
