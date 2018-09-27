#include "cxml_stack.h"
#include <stdlib.h>

struct cxml_stack{
    void *item;
    cxml_stack *next;
};

cxml_stack *cxml_stack_new()
{
    cxml_stack *stack = malloc(sizeof *stack);
    stack->item = NULL;
    stack->next = NULL;
    return stack;
}

void cxml_stack_push(cxml_stack *stack, void *item)
{
    cxml_stack *newitem = malloc(sizeof *newitem);
    newitem->item = item;
    newitem->next = stack->next;
    stack->next = newitem;
}

int cxml_stack_isempty(cxml_stack *stack)
{
    return stack->next == NULL;
}

void *cxml_stack_pop(cxml_stack *stack)
{
    cxml_stack *popitem = stack->next;
    if(popitem == NULL) return NULL;
    stack->next = popitem->next;
    void *item = popitem->item;
    free(popitem);
    return item;
}