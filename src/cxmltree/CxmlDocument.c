#include "CxmlDocument.h"
#include <stdlib.h>



CXmlDocument *CXmlDocument_new()
{
    CXmlDocument *doc = malloc(sizeof *doc);
    if(doc == NULL) return NULL;

    doc->docnode = CXmlNode_new(CXML_DOCUMENT, NULL, NULL, NULL);
    if(doc->docnode == NULL){
        free(doc);
        return NULL;
    }

    doc->root = NULL;
    return doc;
}

int CXmlDocument_setRoot(CXmlDocument *doc, CXmlNode *node)
{
    if(doc == NULL || node == NULL) return CXML_ENULLARG;
    if(doc->root != NULL) return CXML_EDOCCHILD;

    doc->root = node;
    CXmlNode_appendChild(doc->docnode, node);
    return CXML_ESUCCESS;
}

CXmlDocument *CXmlNode_getDocument(CXmlNode *node)
{
    if(node == NULL) return NULL;
    CXmlNode *root = CXmlNode_getRoot(node);

    return root != NULL && root->type == CXML_DOCUMENT
           ? (CXmlDocument *)root
           : NULL;
}

void CXmlDocument_destroy(void *document) {
    if(document == NULL) return;
    CXmlDocument *doc = document;
    CXmlNode_destroyTree(doc->docnode);
    free(doc);
}