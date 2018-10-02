#include "CXmlQueue.h"
#include "CXmlPreProcessor.h"
#include <stdlib.h>

struct CXmlQueueNode{
    void *item;
    struct CXmlQueueNode *next;
};

struct CXmlQueue{
    struct CXmlQueueNode *head;
    struct CXmlQueueNode *tail;
};

CXmlQueue *CXmlQueue_new()
{
    CXmlQueue *queue = malloc(sizeof *queue);
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

static struct CXmlQueueNode *createNewNode(void *item)
{
    struct CXmlQueueNode *node = malloc(sizeof *node);
    node->item = item;
    node->next = NULL;
    return node;
}

void CXmlQueue_enqueue(CXmlQueue *queue, void *item)
{
    struct CXmlQueueNode *newnode = createNewNode(item);
    if(CXML_ISNULL(queue->tail)){
        queue->head = newnode;
        queue->tail = newnode;

    }else{
        queue->tail->next = newnode;
        queue->tail = newnode;
    }
}

int CXmlQueue_isEmpty(CXmlQueue *queue)
{
    return CXML_ISNULL(queue->head);
}

void *CXmlQueue_dequeue(CXmlQueue *queue)
{
    if(CXML_ISNULL(queue->head)) return NULL;

    struct CXmlQueueNode *node = queue->head;
    void *item = node->item;

    queue->head = queue->head->next;

    if(CXML_ISNULL(queue->head)) queue->tail = NULL;

    //free(node);
    return item;
}
