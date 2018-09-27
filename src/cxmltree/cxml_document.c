//
// Created by c14lrl on 2018-09-21.
//

#include "cxml_document.h"
#include <stdlib.h>



cxml_document *cxml_document_new()
{
    cxml_document *doc = malloc(sizeof *doc);
    if(doc == NULL) return NULL;

    doc->docnode = cxml_node_new(CXML_DOCUMENT, NULL, NULL, NULL);
    if(doc->docnode == NULL){
        free(doc);
        return NULL;
    }

    doc->root = NULL;
    return doc;
}

int cxml_document_setRoot(cxml_document *doc, cxml_node *node)
{
    if(doc == NULL || node == NULL) return CXML_ENULLARG;
    if(doc->root != NULL) return CXML_EDOCCHILD;

    doc->root = node;
    cxml_node_appendChild(doc->docnode, node);
    return CXML_ESUCCESS;
}

cxml_document *cxml_node_getDocument(cxml_node *node)
{
    if(node == NULL) return NULL;
    cxml_node *root = cxml_node_getRoot(node);

    return root != NULL && root->type == CXML_DOCUMENT
           ? (cxml_document *)root
           : NULL;
}

void cxml_document_destroy(void *document) {
    if(document == NULL) return;
    cxml_document *doc = document;
    cxml_node_destroyTree(doc->docnode);
    free(doc);
}