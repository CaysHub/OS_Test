#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
pthread_mutex_t lock;
int array[]={56,7,32,44,5,66,9,99,76,33,12,0};
struct param {
    int start;
    int end;
};
void *selectSort(void *arg){
    struct param *param;
    param = (struct param *)arg;
    int start = param->start;
    int end = param->end;
    int i,j,min,minIndex;
    for(i = start; i <= end; i++){
        min = array[i]; 
        minIndex = i;           
        for(j = i; j <= end; j++){      
            if(array[j] < min){                         
                min = array[j];                                             
                minIndex = j;                                                                
            }                                                                                          }                                                                                             if(minIndex != i){
             pthread_mutex_lock(&lock);
             int t = array[i];
             array[i] = array[minIndex];
             array[minIndex] = t;
             pthread_mutex_unlock(&lock);                                                              }                                                                                         }  
}
void select_sort(int start, int end){
    int i,j,min,minIndex;
    for(i = start; i <= end; i++){
        min = array[i];
        minIndex = i;
        for(j = i; j <= end; j++){
            if(array[j] < min){
                min = array[j];
                minIndex = j;
            }
        }
        if(minIndex != i){
            pthread_mutex_lock(&lock);
            int t = array[i];
            array[i] = array[minIndex];
            array[minIndex] = t;
            pthread_mutex_unlock(&lock);
        }
    }
}
void meger_array(int n){
    int *a = array;
    int *b = (int *)malloc(n*sizeof(int));
    int *tmp = b;
    int i;
    int start1 = 0, end1 = n / 2 - 1;
    int start2 = n / 2, end2 = n - 1;
    i=0;
    while(start1 <= end1 && start2 <= end2){
        tmp[i++]=array[start1] < array[start2] ? array[start1++] : array[start2++];
    }
    while(start1 <= end1){
        tmp[i++] = array[start1++];
    }
    while(start2 <= end2){
        tmp[i++] = array[start2++];
    }
    int j;
    for(j = 0; j < i; j++){
        array[j] = b[j];
    }
}
void print_array(int n){
    int i;
    for(i = 0; i < n; i++){
        printf("%d ",array[i]);
    }
    printf("\n");
}
int main(){
    pthread_t worker_tid;
    struct param param;
    int n = sizeof(array) / sizeof(int);
    print_array(n);
    param.start = n / 2;
    param.end = n - 1;
    pthread_mutex_init(&lock,NULL);
    pthread_create(&worker_tid,NULL,selectSort,&param);

    select_sort(0,n / 2 -1);
    pthread_join(worker_tid,NULL);
    pthread_mutex_destroy(&lock);
    print_array(n);
    meger_array(n);
    print_array(n);
    return 0;
}
