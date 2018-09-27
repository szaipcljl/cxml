//
// Created by c14lrl on 2018-09-21.
//

#include "cxml_node.h"
#include <stdlib.h>
#include <string.h>

struct cxml_nodeprivate{
    cxml_node public;
    int dhndl;
    int *error;
};

#define CXML_NODE_COPY_TEXT(src, dest)          \
    do{                                         \
        if(dest != NULL){                       \
            src = malloc(strlen(dest)+1);       \
            strcpy(src, dest);                  \
        }else{                                  \
            src = NULL;                         \
        }                                       \
    }while(0)

static struct cxml_nodeprivate *cxml_node_getpriv(cxml_node *node);

cxml_node *cxml_node_new(cxml_node_type type, const char *name, const char *itxt, cxml_node *parent)
{
    if(parent == NULL && type != CXML_DOCUMENT){
        return NULL;
    }

    struct cxml_nodeprivate *private = malloc(sizeof *private);
    if(private == NULL){
        if(parent != NULL){
            *cxml_node_getpriv(parent)->error = CXML_EALLOC;
        }
        return NULL;
    }

    char *tmpname;
    char *tmpitxt;
    CXML_NODE_COPY_TEXT(tmpname, name);
    CXML_NODE_COPY_TEXT(tmpitxt, itxt);

    struct cxml_node tmp = {
        .type = type,
        .name = tmpname,
        .innerText = tmpitxt,
        .parent = parent,
        .child = NULL
    };
    memcpy(&private->public, &tmp, sizeof tmp);

    if(type == CXML_DOCUMENT){
        private->error = &private->dhndl;
    }else{
        private->error = cxml_node_getpriv(parent)->error;
    }

    *private->error = CXML_ESUCCESS;
    return &private->public;
}

int cxml_node_appendchild(cxml_node *node, cxml_node *child)
{
    node->child = child;
    //TODO implement list appending
    return 1;
}

static struct cxml_nodeprivate *cxml_node_getpriv(cxml_node *node)
{
    return CXML_CONTAINER_OF__(node, struct cxml_nodeprivate, public);
}

void cxml_node_destroy(void *node)
{
    if(node == NULL) return;
    struct cxml_nodeprivate *private = cxml_node_getpriv((cxml_node *)node);
    if(private->public.type == CXML_DOCUMENT){
        free((char *)private->public.name);
    }
    free(private);
}

void cxml_node_seterror__(cxml_node *node, int error)
{
    *cxml_node_getpriv(node)->error = error;
}