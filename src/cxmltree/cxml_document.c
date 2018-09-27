//
// Created by c14lrl on 2018-09-21.
//

#include "cxml_document.h"
#include <stdlib.h>



cxml_document *cxml_document_new()
{
    cxml_document *doc = malloc(sizeof *doc);
    doc->node = cxml_node_new(CXML_DOCUMENT, NULL, NULL, NULL);
    doc->root = NULL;
    return doc;
}

void cxml_document_setroot(cxml_document *doc, cxml_node *node)
{
    if(doc->root == NULL) {
        doc->root = node;
        doc->node->child = node;
        cxml_node_seterror__(doc->node, CXML_ESUCCESS);
    }else{
        cxml_node_seterror__(doc->node, CXML_EDOCCHILD);
    }
}