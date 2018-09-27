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
#define CXML_EALLOC (-1)
#define CXML_EDOCCHILD (-2)
#define CXML_ERRNO (-100)
#define CXML_CONTAINER_OF__(ptr, type, member) ((type *)( \
    (char *)(member_type(type, member) *){ ptr } - offsetof(type, member)))

typedef enum cxml_node_type{
    CXML_NODE,
    CXML_DOCUMENT,
}cxml_node_type;

typedef struct cxml_node{
    const cxml_node_type type;
    const char *name;
    const char *innerText;
    const struct cxml_node *parent;
    struct cxml_node *child;
}cxml_node;

cxml_node *cxml_node_new(cxml_node_type type, const char *name, const char *itxt, cxml_node *parent);
int cxml_node_appendchild(cxml_node *node, cxml_node *child);
void cxml_node_seterror__(cxml_node *node, int error);
#endif // _CXML_NODE_H__