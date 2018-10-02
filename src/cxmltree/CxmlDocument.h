#ifndef _CXML_DOCUMENT_H__
#define _CXML_DOCUMENT_H__

#include "CXmlNode.h"

typedef struct CXmlDocument{
    CXmlNode *docnode;
    CXmlNode *root;
}CXmlDocument;

CXmlDocument *CXmlDocument_new();

int CXmlDocument_setRoot(CXmlDocument *doc, CXmlNode *node);

CXmlDocument *CXmlNode_getDocument(CXmlNode *node);

void CXmlDocument_destroy(void *document);

#endif // _CXML_DOCUMENT_H__