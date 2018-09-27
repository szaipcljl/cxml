#ifndef _CXML_DOCUMENT_H__
#define _CXML_DOCUMENT_H__

#include "cxml_node.h"

typedef struct cxml_document{
    cxml_node *node;
    cxml_node *root;
}cxml_document;

cxml_document *cxml_document_new();

void cxml_document_setroot(cxml_document *doc, cxml_node *node);

cxml_document *cxml_node_getDocument(cxml_node *node);

#endif // _CXML_DOCUMENT_H__