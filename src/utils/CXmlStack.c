#include "CXmlStack.h"
#include <stdlib.h>

struct CXmlStack{
    void *item;
    CXmlStack *next;
};

CXmlStack *CXmlStack_new()
{
    CXmlStack *stack = malloc(sizeof *stack);
    stack->item = NULL;
    stack->next = NULL;
    return stack;
}

void CXmlStack_push(CXmlStack *stack, void *item)
{
    CXmlStack *newitem = malloc(sizeof *newitem);
    newitem->item = item;
    newitem->next = stack->next;
    stack->next = newitem;
}

int CXmlStack_isEmpty(CXmlStack *stack)
{
    return stack->next == NULL;
}

void *CXmlStack_pop(CXmlStack *stack)
{
    CXmlStack *popitem = stack->next;
    if(popitem == NULL) return NULL;
    stack->next = popitem->next;
    void *item = popitem->item;
    free(popitem);
    return item;
}