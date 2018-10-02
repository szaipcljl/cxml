#include "cxml_queue.h"
#include "cxml_preproc.h"
#include <stdlib.h>

struct cxml_queuenode{
    void *item;
    struct cxml_queuenode *next;
};

struct cxml_queue{
    struct cxml_queuenode *head;
    struct cxml_queuenode *tail;
};

CXmlQueue *cxml_queue_new()
{
    CXmlQueue *queue = malloc(sizeof *queue);
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

static struct cxml_queuenode *cxml_queue_newnode(void *item)
{
    struct cxml_queuenode *node = malloc(sizeof *node);
    node->item = item;
    node->next = NULL;
    return node;
}

void cxml_queue_enqueue(CXmlQueue *queue, void *item)
{
    struct cxml_queuenode *newnode = cxml_queue_newnode(item);
    if(CXML_ISNULL(queue->tail)){
        queue->head = newnode;
        queue->tail = newnode;

    }else{
        queue->tail->next = newnode;
        queue->tail = newnode;
    }
}

int cxml_queue_isEmpty(CXmlQueue *queue)
{
    return CXML_ISNULL(queue->head);
}

void *cxml_queue_dequeue(CXmlQueue *queue)
{
    if(CXML_ISNULL(queue->head)) return NULL;

    struct cxml_queuenode *node = queue->head;
    void *item = node->item;

    queue->head = queue->head->next;

    if(CXML_ISNULL(queue->head)) queue->tail = NULL;

    //free(node);
    return item;
}
