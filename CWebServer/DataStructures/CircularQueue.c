//
//  CircularQueue.c
//  CWebServer
//
//  Created by Ardalan Owrangi on 9/11/24.
//

#include "CircularQueue.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct __CircularQueue_t
{
    int maxSize;
    int headIndex;
    int tailIndex;
    pthread_mutex_t lock;
    int elements[];
};
size_t sizeofQueueStruct(void){
    return sizeof(CircularQueue);
}
void lock(CircularQueue *q)
{
    int errorCode;
        if ((errorCode = pthread_mutex_lock(&q->lock)) != 0)
        {
            printf("Lock acquisition failed, exiting program. Error code:%d\n", errorCode);
            exit(1);
        }
    return;
}
void unlock(CircularQueue *q)
{
    int errorCode;
        if ((errorCode = pthread_mutex_unlock(&q->lock)) != 0)
        {
            printf("Unlocking failed, exiting program. Error code:%d\n", errorCode);
            exit(1);
        }
    return;
}
void QueueInit(CircularQueue *q, int size)
{
    q->maxSize = size;
    q->headIndex = q->tailIndex = -1;
    pthread_mutex_init(&q->lock, NULL);
    memset(q->elements, 0, size);
}
int QueueEnqueue(CircularQueue *q, int item)
{
    lock(q);
    if ((q->headIndex == 0 && q->tailIndex == q->maxSize - 1) ||
        ((q->tailIndex + 1) % q->maxSize == q->headIndex))
    {
        // Queue is full:
        unlock(q);
        return -1;
    }
    if (++q->tailIndex == q->maxSize)
        q->tailIndex = 0;
    if (q->headIndex == -1)
        q->headIndex = 0;
    q->elements[q->tailIndex] = item;
    unlock(q);
    // printf("Enqueuing: head: %d, tail: %d\n", q->headIndex, q->tailIndex);
    return 0;
}
int QueueDequeue(CircularQueue *q)
{
    lock(q);
    if (q->headIndex == -1)
    {
        // Queue is empty:
        unlock(q);
        return -1;
    }
    int data = q->elements[q->headIndex];
    if (q->headIndex == q->tailIndex)
        q->headIndex = q->tailIndex = -1;
    else if (++q->headIndex == q->maxSize)
    {
        q->headIndex = 0;
    }
    unlock(q);
    // printf("Dequeuing: head: %d, tail: %d\n", q->headIndex, q->tailIndex);
    return data;
}
void resetQueue(CircularQueue *q, int size)
{
    free(q->elements);
    pthread_mutex_destroy(&q->lock);
    QueueInit(q, size);
}
