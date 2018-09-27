#include "cxml_node.h"
#include "../utils/cxml_stack.h"
#include <stdlib.h>
#include <string.h>

struct cxml_nodelist{
    cxml_node *node;
    cxml_nodelist *next;
    cxml_nodelist *prev;
};

struct cxml_nodeprivate{
    cxml_node public;
    int dhndl;
    int *error;
    unsigned nchild;
};

#define CXML_NODE_COPY_TEXT(src, dest)              \
    do{                                             \
        if((dest) != NULL){                         \
            (src) = malloc(strlen(dest)+1);         \
            strcpy(src, dest);                      \
        }else{                                      \
            (src) = NULL;                           \
        }                                           \
    }while(0)

static inline struct cxml_nodeprivate *cxml_node_getPriv(cxml_node *node);

cxml_node *cxml_node_new(cxml_node_type type, const char *tag, const char *itxt, cxml_node *parent)
{
    if(parent == NULL && type != CXML_DOCUMENT){
        return NULL;
    }

    struct cxml_nodeprivate *private = malloc(sizeof *private);
    if(private == NULL){
        if(parent != NULL){
            *cxml_node_getPriv(parent)->error = CXML_EALLOC;
        }
        return NULL;
    }

    char *tmptag;
    char *tmpitxt;
    CXML_NODE_COPY_TEXT(tmptag, tag);
    CXML_NODE_COPY_TEXT(tmpitxt, itxt);

    struct cxml_node tmp = {
        .type = type,
        .tag = tmptag,
        .innerText = tmpitxt,
        .parent = parent,
        .children = NULL
    };

    memcpy(&private->public, &tmp, sizeof tmp);

    private->public.children = malloc(sizeof *private->public.children);
    private->public.children->node = NULL;
    private->public.children->next = NULL;
    private->public.children->prev = NULL;
    private->nchild = 0;

    if(type == CXML_DOCUMENT){
        private->error = &private->dhndl;
    }else{
        private->error = cxml_node_getPriv(parent)->error;
    }

    *private->error = CXML_ESUCCESS;
    return &private->public;
}

int cxml_node_hasChildren(cxml_node *node)
{
    return cxml_node_getPriv(node)->nchild > 0;
}

unsigned cxml_node_childCount(cxml_node *node)
{
    return cxml_node_getPriv(node)->nchild;
}

int cxml_node_appendChild(cxml_node *node, cxml_node *child)
{
    if(node == NULL) return CXML_ENULLARG;
    if(child == NULL){
        cxml_node_seterror__(node, CXML_ENULLARG);
        return CXML_EFAILURE;
    }

    struct cxml_nodeprivate *private = cxml_node_getPriv(node);

    struct cxml_nodelist *newitem = malloc(sizeof *newitem);
    if(newitem == NULL){
        cxml_node_seterror__(node, CXML_EALLOC);
        return CXML_EFAILURE;
    }
    newitem->node = child;
    newitem->next = NULL;

    if(private->nchild == 0){
        node->children->next = newitem;
        node->children->prev = newitem;
        newitem->prev = NULL;
    }else {
        newitem->prev = node->children->prev;
        node->children->prev->next = newitem;
        node->children->prev = newitem;
    }
    private->nchild++;
    return CXML_ESUCCESS;
}

cxml_node *cxml_node_getChild(cxml_node *node, unsigned index)
{
    if(node == NULL){
        cxml_node_seterror__(node, CXML_ENULLARG);
        return NULL;
    }

    if(index >= cxml_node_getPriv(node)->nchild){
        cxml_node_seterror__(node, CXML_EOUTBOUNDS);
        return NULL;
    }

    cxml_nodelist *curchild = node->children->next;
    if(curchild == NULL) return NULL;

    for(unsigned i = 0; i < index; i++){
        curchild = curchild->next;
    }

    cxml_node_seterror__(node, CXML_ESUCCESS);
    return curchild->node;
}

cxml_node *cxml_node_getChildByTag(cxml_node *node, const char *tag)
{
    if(node == NULL || tag == NULL){
        cxml_node_seterror__(node, CXML_ENULLARG);
        return NULL;
    }

    cxml_nodelist *curchild = node->children->next;
    int found = 0;
    while(curchild != NULL){
        if(strcmp(curchild->node->tag, tag) == 0){
            found = 1;
            break;
        }
        curchild = curchild->next;
    }
    return found ? curchild->node : NULL;
}

cxml_node **cxml_node_getChildrenByTag(cxml_node *node, const char *tag,
                                       unsigned *nchild)
{
    if(node == NULL || tag == NULL){
        cxml_node_seterror__(node, CXML_ENULLARG);
        return NULL;
    }

    cxml_stack *stack = cxml_stack_new();
    *nchild = 0;
    cxml_nodelist *curchild = node->children->next;
    while(curchild != NULL){
        if(strcmp(curchild->node->tag, tag) == 0){
            cxml_stack_push(stack, curchild->node);
            (*nchild)++;
        }
        curchild = curchild->next;
    }

    if(*nchild == 0) return NULL;

    cxml_node **arr = malloc((sizeof *arr) * (*nchild));
    for(int i = (*nchild)-1; i >= 0; i--){
        arr[i] = cxml_stack_pop(stack);
    }
    cxml_stack_destroy(stack);
    return arr;
}

cxml_node *cxml_node_getRoot(cxml_node *node)
{
    return &CXML_CONTAINER_OF__(cxml_node_getPriv(node)->error,
                                struct cxml_nodeprivate,
                                dhndl)->public;
}

void cxml_node_destroy(void *node)
{
    if(node == NULL) return;
    struct cxml_nodeprivate *private = cxml_node_getPriv(node);
    free((char *)private->public.tag);
    free((char *)private->public.innerText);
    free(private->public.children);
    free(private);
}

void cxml_node_destroyTree(void *node)
{
    if(node == NULL) return;
    cxml_stack *stack = cxml_stack_new();

    cxml_node *curnode;
    cxml_nodelist *curchild;
    cxml_nodelist *tmpfree;

    cxml_stack_push(stack, node);
    while(!cxml_stack_isempty(stack)){
        curnode = cxml_stack_pop(stack);
        curchild = curnode->children->next;

        while(curchild != NULL){
            cxml_stack_push(stack, curchild->node);
            tmpfree = curchild;
            curchild = curchild->next;
            free(tmpfree);
        }

        cxml_node_destroy(curnode);
    }
    cxml_stack_destroy(stack);
}

void cxml_node_seterror__(cxml_node *node, int error)
{
    *cxml_node_getPriv(node)->error = error;
}

static inline struct cxml_nodeprivate *cxml_node_getPriv(cxml_node *node)
{
    return CXML_CONTAINER_OF__(node, struct cxml_nodeprivate, public);
}