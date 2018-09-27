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

typedef enum cxml_node_type{
    CXML_NODE,
    CXML_DOCUMENT,
}cxml_node_type;

typedef struct cxml_nodelist cxml_nodelist;

typedef struct cxml_node{
    const cxml_node_type type;
    const char *tag;
    const char *innerText;
    const struct cxml_node *parent;
    cxml_nodelist *children;
}cxml_node;

cxml_node *cxml_node_new(cxml_node_type type, const char *tag, const char *itxt, cxml_node *parent);
int cxml_node_hasChildren(cxml_node *node);
unsigned cxml_node_childCount(cxml_node *node);
int cxml_node_appendChild(cxml_node *node, cxml_node *child);
cxml_node *cxml_node_getChild(cxml_node *node, unsigned index);
cxml_node *cxml_node_getChildByTag(cxml_node *node, const char *tag);
cxml_node **cxml_node_getChildrenByTag(cxml_node *node, const char *tag,
                                       unsigned *nchild);

//TODO add recursive getChildByTag and getChildrenByTag

cxml_node *cxml_node_getRoot(cxml_node *node);

void cxml_node_destroy(void *node);
void cxml_node_destroyTree(void *node);

void cxml_node_seterror__(cxml_node *node, int error);
#endif // _CXML_NODE_H__