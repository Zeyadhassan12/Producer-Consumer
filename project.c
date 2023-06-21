
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "12th Assignment.h"
#include <pthread.h>
#include <semaphore.h>
#define HAVE_STRUCT_TIMESPEC


#define BUFFER_SIZE 10000


int buffer[BUFFER_SIZE];
sem_t semaphore1,semaphore2;


void* producer(void* arg);
void* consumer(void* arg);


int main(){
    long thread;
    //initialize the semaphore
    sem_init(&semaphore1,0,1);
    sem_init(&semaphore2,0,1);
    pthread_t producer_tid,consumer_tid;
    //create the first thread
    pthread_create(&producer_tid, NULL, producer , NULL);
    //create the second thread
    pthread_create(&consumer_tid, NULL, consumer , NULL);
    
    pthread_join(producer_tid, NULL);
    pthread_join(consumer_tid, NULL);
    
    sem_destroy(&semaphore1);
    sem_destroy(&semaphore2);
    return 0;
    
    
}


                   
void* producer(void* arg){
        
        int data ;
        for(int i=0; i<BUFFER_SIZE;i++){
            data = rand() % 1001;
            
            sem_wait(&semaphore1);
            buffer[i]=data;
            printf("Producer produced item number %d with the value %d\n",i,data);
            FILE *fp = fopen("Producer activity1","a");
            fprintf(fp,"Producer produced item number %d with the value %d\n",i,data);
            fclose(fp);
            
            sem_post(&semaphore2);
            
        }
        
        pthread_exit(NULL);
     
}
                   
                   

void* consumer(void* arg){
        
        int data;
        for(int i=0; i < BUFFER_SIZE; i++){
            
            sem_wait(&semaphore2);
            data = buffer[i];
            printf("Consumer consumed item number %d with the value %d\n",i,data);
            FILE *fp = fopen("Consumer activity1","a");
            fprintf(fp,"Consumer consumed item number %d with the value %d\n",i,data);
            fclose(fp);
            
            sem_post(&semaphore1);
            
            
        }
        
        pthread_exit(NULL);
}
