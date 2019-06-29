#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>

pthread_cond_t father_apple,mother_orange;
pthread_cond_t empty;
pthread_mutex_t mutex;
int total = -1;
void init_cond(){
    pthread_cond_init(&father_apple,NULL);
    pthread_cond_init(&mother_orange,NULL);
    pthread_cond_init(&empty,NULL);
    pthread_mutex_init(&mutex,NULL);
}
void *father(void *arg){
    while(1){
        pthread_mutex_lock(&mutex);
        if(total != -1){
            pthread_cond_wait(&empty,&mutex);
        }
        puts("put apple");
        total = 0;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&father_apple);
        sleep(1);
    }
    return NULL;
}
void *mother(void *arg){
    while(1){
        pthread_mutex_lock(&mutex);
        if(total != -1){
            pthread_cond_wait(&empty,&mutex);
        }
        puts("put orange");
        total = 1;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&mother_orange);
        sleep(1);
    }
    return NULL;
}
void *son(void *arg){
    while(1){
        pthread_mutex_lock(&mutex);
        if(total != 0){
            pthread_cond_wait(&father_apple,&mutex);
        }
        puts("get apple");
        total = -1;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&empty);
        sleep(1);
    }
    return NULL;
}
void *daugther(void *arg){
    while(1){
        pthread_mutex_lock(&mutex);
        if(total != 1){
            pthread_cond_wait(&mother_orange,&mutex);
        }
        puts("get orange");
        total = -1;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&empty);
        sleep(1);
    }
    return NULL;
}

int main(){
    pthread_t father_t,mother_t,son_t,daugther_t;
    init_cond(); 
    pthread_create(&father_t,NULL,father,NULL);
    pthread_create(&mother_t,NULL,mother,NULL); 
    pthread_create(&son_t,NULL,son,NULL);
    pthread_create(&daugther_t,NULL,daugther,NULL);

    while(1);
    return 0;
}
