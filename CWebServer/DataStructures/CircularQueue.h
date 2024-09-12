//
//  CircularQueue.h
//  CWebServer
//
//  Created by Ardalan Owrangi on 9/11/24.
//

#ifndef CircularQueue_h
#define CircularQueue_h

#include <stdio.h>
typedef struct __CircularQueue_t CircularQueue;
void QueueInit(CircularQueue *q, int size);
int QueueEnqueue(CircularQueue *q, int item);
int QueueDequeue(CircularQueue *q);
size_t sizeofQueueStruct(void);
#endif /* CircularQueue_h */
