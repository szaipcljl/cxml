#ifndef _CXML_STACK_H__
#define _CXML_STACK_H__

typedef struct CXmlStack CXmlStack;

CXmlStack *CXmlStack_new();

void CXmlStack_push(CXmlStack *stack, void *item);

int CXmlStack_isEmpty(CXmlStack *stack);

void *CXmlStack_pop(CXmlStack *stack);

#define CXmlStack_destroy(x) free(x)

#endif // _CXML_STACK_H__