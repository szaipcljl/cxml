#ifndef _CXML_DOCUMENT_H__
#define _CXML_DOCUMENT_H__

#include "cxml_node.h"

typedef struct cxml_document{
    cxml_node *docnode;
    cxml_node *root;
}cxml_document;

cxml_document *cxml_document_new();

int cxml_document_setRoot(cxml_document *doc, cxml_node *node);

cxml_document *cxml_node_getDocument(cxml_node *node);

void cxml_document_destroy(void *document);

#endif // _CXML_DOCUMENT_H__