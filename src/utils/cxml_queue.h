#ifndef CXML_QUEUE_H_
#define CXML_QUEUE_H_

typedef struct CXmlQueue CXmlQueue;

CXmlQueue *cxml_queue_new();

void cxml_queue_enqueue(CXmlQueue *queue, void *item);

int cxml_queue_isEmpty(CXmlQueue *queue);

void *cxml_queue_dequeue(CXmlQueue *queue);

#define cxml_queue_destroy(x) free(x)

#endif /*CXML_QUEUE_H_*/
