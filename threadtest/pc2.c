#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>
#define N 4
#define ITEM_COUNT N*2
char buf1[N]={0},buf2[N]={0};
int in1,out1,in2,out2;
typedef struct{
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}sema_t;

sema_t buf1_mutex,buf2_mutex;
sema_t buf1_empty,buf2_empty;
sema_t buf1_full,buf2_full;

void sema_init(sema_t *sema,int value){
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema) {
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0) 
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema) {
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}
int buf1_is_empty(){
    return in1==out1;
}
int buf1_is_full(){
    return (in1+1)%N==out1;
}
int buf2_is_empty(){
    return in2==out2;
}
int buf2_is_full(){
    return (in2+1)%N==out2;
}
char get_item_buf1(){
    char item;
    item=buf1[out1];
    out1=(out1+1)%N;
    return item;
}
void put_item_buf1(char item){
    buf1[in1]=item;
    in1=(in1+1)%N;
}
char get_item_buf2(){
    char item;
    item=buf2[out2];
    out2=(out2+1)%N;
    return item;
}
void put_item_buf2(char item){
    buf2[in2]=item;
    in2=(in2+1)%N;
}
void *produce(void *arg){
    int i;
    char item;
    for(i=0;i<ITEM_COUNT;i++){
        sema_wait(&buf1_empty);
        sema_wait(&buf1_mutex);
        item='a'+i;
        put_item_buf1(item);
        printf("produce put item:%c\n",item);

        sema_signal(&buf1_mutex);
        sema_signal(&buf1_full);
    }
    return NULL;
} 
void *calculate(void *arg){
    int i;
    char item;
    for(i=0;i<ITEM_COUNT;i++){
        sema_wait(&buf1_full);
        sema_wait(&buf1_mutex);
        item=get_item_buf1();
        printf("calculate get item:%c\n",item);
        sema_signal(&buf1_mutex);
        sema_signal(&buf1_empty);
        //calculate
        sema_wait(&buf2_empty);
        sema_wait(&buf2_mutex);
        item=item-32;
        put_item_buf2(item);
        printf("calculate put item:%c\n",item);
        sema_signal(&buf2_mutex);
        sema_signal(&buf2_full);
    }
    return NULL;
}
void *consume(void *arg){
    int i;
    char item;
    for(i=0;i<ITEM_COUNT;i++){
        sema_wait(&buf2_full);
        sema_wait(&buf2_mutex);
        item=get_item_buf2();
        printf("consume get item:%c\n",item);
        sema_signal(&buf2_mutex);
        sema_signal(&buf2_empty);
    }
}
int main(){
    pthread_t produce_pid,calculate_pid,consume_pid;
    sema_init(&buf1_mutex,1);
    sema_init(&buf2_mutex,1);
    sema_init(&buf1_full,0);
    sema_init(&buf1_empty,N-1);
    sema_init(&buf2_full,0);
    sema_init(&buf2_empty,N-1);

    pthread_create(&produce_pid,NULL,produce,NULL);
    pthread_create(&calculate_pid,NULL,calculate,NULL);
    pthread_create(&consume_pid,NULL,consume,NULL);

    pthread_join(consume_pid,NULL);
    return 0;
}
















