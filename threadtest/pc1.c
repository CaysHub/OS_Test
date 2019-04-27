#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>
#define N 4
#define ITEM_COUNT N*2
char buf1[N] = {0};
char buf2[N] = {0};
int in1,out1;
int in2,out2;
pthread_mutex_t buf1_mutex,buf2_mutex;
pthread_cond_t wait_empty_buf1,wait_empty_buf2;
pthread_cond_t wait_full_buf1,wait_full_buf2;
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
        pthread_mutex_lock(&buf1_mutex);
        while(buf1_is_full()){
            pthread_cond_wait(&wait_empty_buf1,&buf1_mutex);
        }
        item='a'+i;
        put_item_buf1(item);
        printf("produce item:%c\n",item);
        pthread_cond_signal(&wait_full_buf1);
        pthread_mutex_unlock(&buf1_mutex);
    }
    return NULL;
}
void *calculate(void *arg){
    int i;
    char item;
    for(i=0;i<ITEM_COUNT;i++){
        pthread_mutex_lock(&buf1_mutex);
        while(buf1_is_empty()){
            pthread_cond_wait(&wait_full_buf1,&buf1_mutex);
        }
        item=get_item_buf1();
        printf("calculate get item:%c\n",item);
        pthread_cond_signal(&wait_empty_buf1);
        pthread_mutex_unlock(&buf1_mutex);
        //calculate 
        pthread_mutex_lock(&buf2_mutex);
        while(buf2_is_full()){
            pthread_cond_wait(&wait_empty_buf2,&buf2_mutex);
        }
        item=item-32;
        put_item_buf2(item);
        printf("calculate put item:%c\n",item);
        pthread_cond_signal(&wait_full_buf2);
        pthread_mutex_unlock(&buf2_mutex);
    }
    return NULL;
}
void *consume(void *arg){
    int i;
    char item;
    for(i=0;i<ITEM_COUNT;i++){
        pthread_mutex_lock(&buf2_mutex);
        while(buf2_is_empty()){
            pthread_cond_wait(&wait_full_buf2,&buf2_mutex);
        }
        item=get_item_buf2();
        printf("consume get item:%c\n",item);
        pthread_cond_signal(&wait_empty_buf2);
        pthread_mutex_unlock(&buf2_mutex);
    }
    return NULL;
}
int main(){
    pthread_t produce_pid,consume_pid,calculate_pid;
    pthread_mutex_init(&buf1_mutex,NULL);
    pthread_mutex_init(&buf2_mutex,NULL);
    pthread_cond_init(&wait_empty_buf1,NULL);
    pthread_cond_init(&wait_empty_buf2,NULL);
    pthread_cond_init(&wait_full_buf1,NULL);
    pthread_cond_init(&wait_full_buf2,NULL);

    pthread_create(&produce_pid,NULL,produce,NULL);
    pthread_create(&calculate_pid,NULL,calculate,NULL);
    pthread_create(&consume_pid,NULL,consume,NULL);

    pthread_join(consume_pid,NULL);

    pthread_cond_destroy(&wait_full_buf2);
    pthread_cond_destroy(&wait_full_buf1);
    pthread_cond_destroy(&wait_empty_buf2);
    pthread_cond_destroy(&wait_empty_buf1);
    pthread_mutex_destroy(&buf1_mutex);
    pthread_mutex_destroy(&buf2_mutex);
    return 0;
}








