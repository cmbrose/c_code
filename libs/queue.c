#include "queue.h"

#include <stdlib.h>
#include <string.h>

typedef struct qnode
{
	struct qnode *next;
	void *data;
} *qnode_p;

struct queue
{
	qnode_p head;
	qnode_p tail;
	int size;
	int data_size;
};

//----------------------------------------------------------
queue_p queue_make(int data_size)
{
	queue_p q = malloc(sizeof(*q));
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	q->data_size = data_size;

	return q;
}

//----------------------------------------------------------
void queue_destroy(queue_p q)
{
	if (q == NULL)
		return;

	queue_clear(q);
	free(q);
}

//----------------------------------------------------------
void queue_clear(queue_p q)
{
	qnode_p n = q->head;
	while (n != NULL) 
	{
		qnode_p tmp = n;
		n = n->next;
		free(tmp);
	}

	q->head = q->tail = NULL;
	q->size = 0;
}

//----------------------------------------------------------
void queue_push(queue_p q, void *data)
{
	qnode_p node = malloc(sizeof(*node));
	node->data = malloc(q->data_size);
	node->next = NULL;

	memcpy(node->data, data, q->data_size);

	if (q->head == NULL) 
	{
		q->head = q->tail = node;
	}
	else
	{
		q->tail->next = node;
		q->tail = node;
	}

	q->size += 1;
}

//----------------------------------------------------------
void queue_peek(queue_p q, void *out)
{
	if (q->size == 0)
		return;

	memcpy(out, q->head->data, q->data_size);
}

//----------------------------------------------------------
void queue_pop(queue_p q, void *out)
{
	if (q->size == 0)
		return;

	memcpy(out, q->head->data, q->data_size);

	qnode_p node = q->head;
	q->head = q->head->next;

	free(node->data);
	free(node);

	q->size -= 1;

	if (q->size == 0)
	{
		q->head = q->tail = NULL;
	}
}

//----------------------------------------------------------
int queue_size(queue_p q)
{
	return q->size;
}