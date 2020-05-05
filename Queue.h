#include <stdio.h>
#include <stdlib.h>

// some part of the code is taken from :https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
typedef struct Queue
{
        int capacity;  // shows the maximum capacity queue can have
        int size;       // current size of the queue
        int front;      
        int rear;
        int *elements; // elements array is defined here.
}Queue;

Queue * QueueInitialize(int maxElements);

int front(Queue *Q);

int QueueRemove(Queue *Q);

void QueueInsert(Queue *Q,int element);

