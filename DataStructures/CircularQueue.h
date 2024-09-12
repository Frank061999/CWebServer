//
//  CircularQueue.h
//  CWebServer
//
//  Created by Ardalan Owrangi on 9/11/24.
//

#ifndef CircularQueue_h
#define CircularQueue_h

#include <stdio.h>
typedef struct CircularQueue CircularQueue;
CircularQueue* QueueInit(int maxSize);
void QueueEnqueue(CircularQueue* q, int new);
int QueueDequeue(CircularQueue* q);

#endif /* CircularQueue_h */
