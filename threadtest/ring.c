#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 100
#define BUF_N 4
int buf[BUF_N] = {0};
int in = 0, out = 0;
struct param{
    int id;
};
typedef struct{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}sema_t;

sema_t mutexs[N];
sema_t buf_mutex;
sema_t buf_full,buf_empty;

void sema_init(sema_t *sema,int value){
    sema->value=value;
    pthread_mutex_init(&sema->mutex,NULL);
    pthread_cond_init(&sema->cond,NULL);
}
void sema_wait(sema_t *sema){
    pthread_mutex_lock(&sema->mutex);
    while(sema->value<=0)
        pthread_cond_wait(&sema->cond,&sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}
void sema_signal(sema_t *sema){
    pthread_mutex_lock(&sema->mutex);
    sema->value++;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}
int get_item(){
    int item;
    item=buf[out];
    out=(out+1)%BUF_N;
    return item;
}
void put_item(int item){
    buf[in]=item;
    in=(in+1)%BUF_N;
}
void *ring(void *arg){
    int i;
    int item;
    struct param *param=(struct param *)arg;
    i=param->id;
    if(i==0){
        sema_wait(&buf_empty);
        sema_wait(&buf_mutex);
        item=i+1;
        put_item(item);
        sema_signal(&buf_mutex);
        sema_signal(&buf_full);
        sema_signal(&mutexs[(i+1)%N]);

        sema_wait(&mutexs[i]);
        sema_wait(&buf_full);
        sema_wait(&buf_mutex);
        item=get_item();
        printf("thread %d item:%d\n",i+1,item);
        sema_signal(&buf_mutex);
        sema_signal(&buf_empty);
        sema_signal(&mutexs[i]);
    }else{
        sema_wait(&mutexs[i]);
        sema_wait(&buf_full);
        sema_wait(&buf_mutex);
        item=get_item();
        printf("thread %d item:%d\n",i+1,item);
        item++;
        put_item(item);
        sema_signal(&buf_mutex);
        sema_signal(&buf_full);
        sema_signal(&mutexs[(i+1)%N]);
    }
    return NULL;
}
int main(){
    pthread_t threads[N];
    int i;
    struct param params[N];
    sema_init(&buf_mutex,1);
    sema_init(&buf_full,0);
    sema_init(&buf_empty,BUF_N-1);
    for(i=0;i<N;i++){
        sema_init(&mutexs[i],0);
    }

    for(i=0;i<N;i++){
        params[i].id=i;
        pthread_create(&threads[i],NULL,ring,&params[i]);
    }
    pthread_join(threads[0],NULL);
    return 0;
}
