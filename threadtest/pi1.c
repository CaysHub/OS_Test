#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
int n=200000;
double worker_output = 0;
void *worker(void *arg) {
    int i;
    
    for (i = 2*(n/2)-1; i <= 2*n-1; i += 2){
        worker_output += pow(-1 , (i-1) / 2) * 1.0/(i*1.0);
    }

    return NULL;
}
double master_output = 0;
void master() {
    int i;
    for (i = 1; i < 2*(n/2)-1; i += 2 ) {
        master_output += pow(-1 , (i-1) / 2) * 1.0/(i*1.0);
    }
}


int main (int argc, char *argv[]) {
    pthread_t worker_tid;
    double total, pi;
    
    pthread_create(&worker_tid, NULL, worker, NULL);
    master();
    pthread_join(worker_tid, NULL);
    total = master_output + worker_output;
    pi = total * 4;
    printf("CYS PI = %lf\n", pi);
    return 0;
}
