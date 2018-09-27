#ifndef _CXML_STACK_H__
#define _CXML_STACK_H__

typedef struct cxml_stack cxml_stack;

cxml_stack *cxml_stack_new();

void cxml_stack_push(cxml_stack *stack, void *item);

int cxml_stack_isempty(cxml_stack *stack);

void *cxml_stack_pop(cxml_stack *stack);

#define cxml_stack_destroy(x) free(x)

#endif // _CXML_STACK_H__