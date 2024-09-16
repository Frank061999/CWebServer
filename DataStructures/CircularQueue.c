//
//  CircularQueue.c
//  CWebServer
//
//  Created by Ardalan Owrangi on 9/11/24.
//

#include "CircularQueue.h"
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#ifdef __linux__
#include <fcntl.h>
#include <sys/stat.h>
#endif


                                         

typedef struct CircularQueue {
    int count;
    int frontIndex;
    int tailIndex;
    int maxSize;
    pthread_mutex_t headLock;
    pthread_mutex_t tailLock;
    sem_t* space;
    sem_t* items;
    int elements[];
} CircularQueue;

CircularQueue* QueueInit(int maxSize){
    CircularQueue* queue = calloc(1,sizeof(CircularQueue) + sizeof(int) * maxSize);
    queue->frontIndex = 0;
    queue->tailIndex = 0;
    queue->maxSize = maxSize;
    pthread_mutex_init(&queue->headLock, NULL);
    pthread_mutex_init(&queue->tailLock, NULL);
    queue->space = sem_open("/queue_space", O_CREAT, S_IRUSR | S_IWUSR, maxSize);
    queue->items = sem_open("/queue_items", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(queue->items == SEM_FAILED || queue->space == SEM_FAILED) {
        printf("Semaphore open failed: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return queue;
}

void QueueEnqueue(CircularQueue* q, int new){
    sem_wait(q->space);
    pthread_mutex_lock(&q->tailLock);
    q->elements[q->tailIndex] = new;
    q->tailIndex = (q->tailIndex + 1) % q->maxSize;
    sem_post(q->items);
    pthread_mutex_unlock(&q->tailLock);
}

int QueueDequeue(CircularQueue* q){
    sem_wait(q->items);
    pthread_mutex_lock(&q->headLock);
    int returnValue = q->elements[q->frontIndex];
    q->frontIndex = (q->frontIndex + 1) % q->maxSize;
    sem_post(q->space);
    pthread_mutex_unlock(&q->headLock);
    return returnValue;
}

void DestroyQueue(CircularQueue* q){
    sem_close(q->space);
    sem_close(q->items);
    sem_unlink("/queue_space");
    sem_unlink("/queue_items");
    pthread_mutex_destroy(&q->headLock);
    pthread_mutex_destroy(&q->tailLock);
}
