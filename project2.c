


#include "project2.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define HAVE_STRUCT_TIMESPEC

#define PRODUCER_NUM 4
#define CONSUMER_NUM 4
#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
sem_t semaphore , full , empty;

void* producer(void* arg);
void* consumer(void* arg);
int pos_in = 0;
int pos_out = 0;

int main(){
    
    
    sem_init(&semaphore,0,1);
    sem_init(&empty,0,BUFFER_SIZE);
    sem_init(&full,0,0);
    
    int producer_id[PRODUCER_NUM];
    int consumer_id[CONSUMER_NUM];
    
    pthread_t producers [PRODUCER_NUM];
    for(int i=0; i< PRODUCER_NUM ; i++){
        producer_id[i] = i ;
        pthread_create(&producers[i],NULL,producer,&producer_id[i]);
        
    }
    
    pthread_t consumers [CONSUMER_NUM];
    for(int i=0 ; i < CONSUMER_NUM ; i++){
        consumer_id[i] = i;
        pthread_create(&consumers[i],NULL,consumer,&consumer_id[i]);

    }
    
    
    for(int i=0; i< PRODUCER_NUM ; i++){
        pthread_join(producers[i],NULL);
        
    }
    
    for(int i=0 ; i < CONSUMER_NUM ; i++){
        pthread_join(consumers[i],NULL);

    }
    
    sem_destroy(&semaphore);
    sem_destroy(&full);
    sem_destroy(&empty);
    
    return 0;
}


void* producer(void* arg){
    int id = *(int*)arg;
    int data;
    for(int i=0;i<10000;i++){
        data = rand() % 1001;
        sem_wait(&empty);
        sem_wait(&semaphore);
        
        
        buffer[pos_in]=data;
        pos_in = (pos_in + 1)%10;
        FILE *fp = fopen("Producer activity2","a");
        fprintf(fp,"Producer number %d produced item number %d with the value %d\n",id,i,data);
        
        fclose(fp);
        sem_post(&semaphore);
        sem_post(&full);
        
    }
    pthread_exit(NULL);
    
}

void* consumer(void* arg){
    int id  = *(int*)arg;
    int data;
    for(int i = 0;i<10000 ; i++){
        
        sem_wait(&full);
        sem_wait(&semaphore);
        
        
        data = buffer[pos_out];
        pos_out = (pos_out + 1)%10;
        FILE *fp = fopen("Consumer activity2","a");
        fprintf(fp,"Consumer number %d consumed item number %d with the value %d\n",id,i,data);
            
        fclose(fp);
        sem_post(&semaphore);
        sem_post(&empty);
        
    }
    pthread_exit(NULL);
}








