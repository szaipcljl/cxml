#include "CXmlString.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <zconf.h>
#include <ctype.h>

#define CXMLSTRING_HDR_MARKER char
#define CXMLSTRING_HDR_MARKER_SIZE sizeof(CXMLSTRING_HDR_MARKER)

#define CXMLSTRING_ACCESS_HDR(size, str) \
    ((struct CXmlStringHdr##size*)((str)-sizeof(struct CXmlStringHdr##size)))

#define CXMLSTRING_GET_HDRTYPE(str) \
    ((enum CXmlStringHdrType)(*(CXMLSTRING_HDR_MARKER*)((str)-1)))

#define CXMLSTRING_GET_HDRSIZE(str) \
    (CXMLSTRING_GET_HDRTYPE(str)*8)

#define CXMLSTRING_APPLY_TERMINATOR(str, len)\
    do{(str)[len] = '\0';}while(0)

#define CXmlString_getLen(str, out_len)\
    do{\
        switch(CXMLSTRING_GET_HDRTYPE(str)){\
            case CXMLSTRING_HDR8:  \
                (out_len) = CXMLSTRING_ACCESS_HDR(8, str)->len;\
                break;\
            case CXMLSTRING_HDR16:  \
                (out_len) = CXMLSTRING_ACCESS_HDR(16, str)->len;\
                break;\
            case CXMLSTRING_HDR32:  \
                (out_len) = CXMLSTRING_ACCESS_HDR(32, str)->len;\
                break;\
            case CXMLSTRING_HDR64:  \
                (out_len) = CXMLSTRING_ACCESS_HDR(64, str)->len;\
                break;\
            default:\
                (out_len) = 0;\
        }\
    }while(0)

#define CXmlString_getSize(str, out_size)\
    do{\
        switch(CXMLSTRING_GET_HDRTYPE(str)){\
            case CXMLSTRING_HDR8:  \
                (out_size) = CXMLSTRING_ACCESS_HDR(8, str)->len;\
                break;\
            case CXMLSTRING_HDR16:  \
                (out_size) = CXMLSTRING_ACCESS_HDR(16, str)->len;\
                break;\
            case CXMLSTRING_HDR32:  \
                (out_size) = CXMLSTRING_ACCESS_HDR(32, str)->len;\
                break;\
            case CXMLSTRING_HDR64:  \
                (out_size) = CXMLSTRING_ACCESS_HDR(64, str)->len;\
                break;\
            default:\
                (out_size) = 0;\
        }\
    }while(0)

#define CXmlString_setLen(str, newlen)                                          \
    do{                                                                         \
        switch(CXMLSTRING_GET_HDRTYPE(str)){                                    \
            case CXMLSTRING_HDR8:                                               \
                CXMLSTRING_ACCESS_HDR(8, str)->len = (uint8_t)(newlen);         \
                break;                                                          \
            case CXMLSTRING_HDR16:                                              \
                CXMLSTRING_ACCESS_HDR(16, str)->len = (uint16_t)(newlen);       \
                break;                                                          \
            case CXMLSTRING_HDR32:                                              \
                CXMLSTRING_ACCESS_HDR(32, str)->len = (uint32_t)(newlen);       \
                break;                                                          \
            case CXMLSTRING_HDR64:                                              \
                CXMLSTRING_ACCESS_HDR(64, str)->len = newlen;                   \
                break;                                                          \
        }                                                                       \
    }while(0)

#define CXmlString_setSize(str, newsize)                                        \
    do{                                                                         \
        switch(CXMLSTRING_GET_HDRTYPE(str)){                                    \
            case CXMLSTRING_HDR8:                                               \
                CXMLSTRING_ACCESS_HDR(8, str)->size = (uint8_t)(newsize);       \
                break;                                                          \
            case CXMLSTRING_HDR16:                                              \
                CXMLSTRING_ACCESS_HDR(16, str)->size = (uint16_t)(newsize);     \
                break;                                                          \
            case CXMLSTRING_HDR32:                                              \
                CXMLSTRING_ACCESS_HDR(32, str)->size = (uint32_t)(newsize);     \
                break;                                                          \
            case CXMLSTRING_HDR64:                                              \
                CXMLSTRING_ACCESS_HDR(64, str)->size = newsize;                 \
                break;                                                          \
        }                                                                       \
    }while(0)

enum CXmlStringHdrType{
    CXMLSTRING_HDR8  = 1,
    CXMLSTRING_HDR16 = 2,
    CXMLSTRING_HDR32 = 4,
    CXMLSTRING_HDR64 = 8
};

struct CXmlStringHdr8{
    uint8_t len;
    uint8_t size;
};

struct CXmlStringHdr16{
    uint16_t len;
    uint16_t size;
};

struct CXmlStringHdr32{
    uint32_t len;
    uint32_t size;
};

struct CXmlStringHdr64{
    uint64_t len;
    uint64_t size;
};

static inline size_t CXmlString_getHdrSize(enum CXmlStringHdrType hdrType)
{
    switch(hdrType){
        case CXMLSTRING_HDR8:  return sizeof(struct CXmlStringHdr8);
        case CXMLSTRING_HDR16: return sizeof(struct CXmlStringHdr16);
        case CXMLSTRING_HDR32: return sizeof(struct CXmlStringHdr32);
        case CXMLSTRING_HDR64: return sizeof(struct CXmlStringHdr64);
        default:               return 0;
    }
}

static inline CXmlString *CXmlString_alloc(size_t size, enum CXmlStringHdrType hdrType)
{
    size_t hdrSize = CXmlString_getHdrSize(hdrType);
    CXmlString *ptr = malloc(size+hdrSize+CXMLSTRING_HDR_MARKER_SIZE);
    *(CXMLSTRING_HDR_MARKER*)(ptr+hdrSize) = hdrType;
    return ptr+hdrSize+CXMLSTRING_HDR_MARKER_SIZE;
}

static inline enum CXmlStringHdrType CXmlString_getHdrType(size_t size)
{
    if(size < 1 << 8U) return CXMLSTRING_HDR8;
    if(size < 1 << 16) return CXMLSTRING_HDR16;
#if (LONG_MAX == LLONG_MAX)
    if(size < 1ll<<32) return CXMLSTRING_HDR32;
    return CXMLSTRING_HDR64;
#else
    return CXMLSTRING_HDR32;
#endif
}

//static CXmlString

CXmlString *CXmlString_new(const char *str)
{
    return CXmlString_newLen(str, strlen(str));
}

CXmlString *CXmlString_newLen(const char *str, size_t len)
{
    enum CXmlStringHdrType type = CXmlString_getHdrType(len);
    CXmlString *newstr = CXmlString_alloc(len, type);
    memset(newstr, '\0', len);
    memcpy(newstr, str, len);
    CXMLSTRING_APPLY_TERMINATOR(newstr, len);
    CXmlString_setLen(newstr, strlen(newstr));
    CXmlString_setSize(newstr, len);
    return newstr;
}

CXmlString *CXmlString_empty()
{
    return CXmlString_newLen("", 0);
}

size_t CXmlString_len(const CXmlString *str)
{
    size_t len;
    CXmlString_getLen(str, len);
    return len;
}

int CXmlString_isEmpty(const CXmlString *str)
{
    size_t len;
    CXmlString_getLen(str, len);
    return len == 0;
}

CXmlString *CXmlString_dup(const CXmlString *str)
{
    size_t len;
    CXmlString_getLen(str, len);
    return CXmlString_newLen(str, len);
}

CXmlString *CXmlString_trim(const CXmlString *str)
{
    NOT_IMPLEMENTED //TODO Implement
}

CXmlString *CXmlString_copy(CXmlString *dest, const CXmlString *src)
{
    size_t destSize;
    size_t srcLen;
    CXmlString_getSize(dest, destSize);
    CXmlString_getLen(src, srcLen);
    if(srcLen > destSize){
        //Need to realloc
        enum CXmlStringHdrType newHdrType = CXmlString_getHdrType(srcLen);
        CXmlString_realloc(dest, srcLen, newHdrType);
    }

    memcpy(dest, src, srcLen);
    CXmlString_setSize(dest, srcLen);
    CXmlString_setLen(dest, srcLen);
    return dest;
}

CXmlString *CXmlString_ncopy(CXmlString *dest, const CXmlString *src, size_t len)
{
    NOT_IMPLEMENTED //TODO Implement
}