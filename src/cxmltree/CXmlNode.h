#ifndef _CXML_NODE_H__
#define _CXML_NODE_H__

#if defined(__GNUC__) || defined(__clang__)
#define member_type(type, member) __typeof__ (((type *)0)->member)
#else
#define member_type(type, member) const void
#endif

#include <stddef.h>

#define CXML_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define CXML_DEBUGF(msg, ...)\
    printf("[DEBUG] %s::%s:%d | "msg"\n", CXML_FILENAME__, __func__, __LINE__, __VA_ARGS__)
#define CXML_DEBUG(msg) CXML_DEBUGF("%s", msg)

#define CXML_ESUCCESS 0
#define CXML_EFAILURE (-1)
#define CXML_EALLOC (-2)
#define CXML_EDOCCHILD (-3)
#define CXML_ENULLARG (-4)
#define CXML_EOUTBOUNDS (-5)
#define CXML_ERRNO (-100)
#define CXML_CONTAINER_OF__(ptr, type, member) ((type *)( \
    (char *)(member_type(type, member) *){ ptr } - offsetof(type, member)))

typedef enum CXmlNodeType{
    CXML_NODE,
    CXML_DOCUMENT,
    CXML_ELEMENT,
    CXML_TEXT,
    CXML_COMMENT,
    CXML_ATTRIBUTE
}CXmlNodeType;

typedef struct CXmlNodeList CXmlNodeList;

typedef struct CXmlNode{
    const CXmlNodeType type;
    const char *tag;
    const char *text;
    const struct CXmlNode *parent;
    CXmlNodeList *children;
}CXmlNode;

CXmlNode *CXmlNode_new(CXmlNodeType type, const char *tag, const char *itxt,
                       CXmlNode *parent);
int CXmlNode_hasChildren(CXmlNode *node);
unsigned CXmlNode_childCount(CXmlNode *node);
int CXmlNode_appendChild(CXmlNode *node, CXmlNode *child);
CXmlNode *CXmlNode_getChild(CXmlNode *node, unsigned index);
CXmlNode *CXmlNode_getChildByTag(CXmlNode *node, const char *tag);
char *CXmlNode_getInnerText(CXmlNode *node);
char *CXmlNode_getInnerXml(CXmlNode *node);
CXmlNode **CXmlNode_getChildrenByTag(CXmlNode *node, const char *tag,
                                     unsigned *nchild);

//TODO add recursive getChildByTag and getChildrenByTag

CXmlNode *CXmlNode_getRoot(CXmlNode *node);

void CXmlNode_destroy(void *node);
void CXmlNode_destroyTree(void *node);

void cxml_node_seterror__(CXmlNode *node, int error);
#endif // _CXML_NODE_H__