#ifndef CXML_QUEUE_H_
#define CXML_QUEUE_H_

typedef struct CXmlQueue CXmlQueue;

CXmlQueue *CXmlQueue_new();

void CXmlQueue_enqueue(CXmlQueue *queue, void *item);

int CXmlQueue_isEmpty(CXmlQueue *queue);

void *CXmlQueue_dequeue(CXmlQueue *queue);

#define cxml_queue_destroy(x) free(x)

#endif /*CXML_QUEUE_H_*/
