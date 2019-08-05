#ifndef QUEUE_H
#define QUEUE_H

typedef struct queue *queue_p;

/*
    Initializes a new queue that is ready to use
*/
queue_p queue_make(int data_size);

/*
    Deallocates all memory associated with the queue
    The queue is no longer usable.
*/
void queue_destroy(queue_p q);

/*
    Empties the queue and deallocates memory as neccesary.  
    The queue is still usable.
*/
void queue_clear(queue_p q);

/*
    Adds a copy of the given data to the queue
*/
void queue_push(queue_p q, void *data);

/*
    Writes the first entry in the queue to the given pointer.
    Returns immediately if the queue is empty.
*/
void queue_peek(queue_p q, void *out);

/*
    Writes the first entry in the queue to the given pointer and removes the entry.
    Returns immediately if the queue is empty.
*/
void queue_pop(queue_p q, void *out);

/*
    Returns the nubmer of elements stored in the queue.
*/
int queue_size(queue_p q);

#endif // QUEUE_H