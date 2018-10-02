#ifndef CXML_PREPROCESSOR_H_
#define CXML_PREPROCESSOR_H_

#define CXML_ISNULL(ptr) ((ptr) == NULL)

#define CXML_NOTNULL(ptr) ((ptr) == NULL)

#define CXML_RET_IFNULL(ptr, rettype)\
    if(CXML_ISNULL(ptr)){       \
        cxmlerrno=CXML_ENULLARG;\
        return rettype;                 \
    }
#define CXML_RETNULL_IFNULL(ptr) CXML_RET_IFNULL(ptr, NULL)
#define CXML_RETVOID_IFNULL(ptr) CXML_RET_IFNULL(ptr,)


#if defined(__GNUC__) || defined(__clang__)
#define CXML_MEMBER_TYPE__(type, member) __typeof__ (((type *)0)->member)
#else
#define CXML_MEMBER_TYPE__(type, member) const void
#endif

#define CXML_CONTAINER_OF__(ptr, type, member) ((type *)( \
    (char *)(CXML_MEMBER_TYPE__(type, member) *){ ptr } - offsetof(type, member)))


//TODO envelope in #ifdef DEBUG
#define CXML_FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define CXML_DEBUGF(msg, ...)\
    printf("[DEBUG] %s::%s:%d | "msg"\n", CXML_FILENAME__, __func__, __LINE__, __VA_ARGS__)

#define CXML_DEBUG(msg) CXML_DEBUGF("%s", msg)

#endif /*CXML_PREPROCESSOR_H_*/
