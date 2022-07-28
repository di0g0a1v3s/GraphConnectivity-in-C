#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct Queue* Queue;

Queue createQueue();
void enQueue(Queue q, int k);
int deQueue(Queue q); 
int isQueueEmpty(Queue q);

#endif