#include "CXmlString.h"
#include "CXmlPreProcessor.h"
#include "CXmlErrno.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define CXMLS_HDR_MARKER char
#define CXMLS_HDR_MARKER_SIZE sizeof(CXMLS_HDR_MARKER)

#define CXMLS_ACCESS_HDR(size, str) \
    ((struct CXmlStringHdr##size*)((str)-(sizeof(struct CXmlStringHdr##size)+CXMLS_HDR_MARKER_SIZE)))

#define CXMLS_GET_HDRTYPE(str) \
    ((enum CXmlStringHdrType)(*(CXMLS_HDR_MARKER*)((str)-1)))

#define CXMLS_GET_HDRSIZE(str) \
    (CXMLS_GET_HDRTYPE(str)*8)

#define CXMLS_APPLY_TERMINATOR(str, len)\
    do{(str)[len] = '\0';}while(0)

#define CXMLS_GET_LEN(str, out_len)\
    do{\
        switch(CXMLS_GET_HDRTYPE(str)){\
            case CXMLSTRING_HDR8:  \
                (out_len) = CXMLS_ACCESS_HDR(8, str)->len;\
                break;\
            case CXMLSTRING_HDR16:  \
                (out_len) = CXMLS_ACCESS_HDR(16, str)->len;\
                break;\
            case CXMLSTRING_HDR32:  \
                (out_len) = CXMLS_ACCESS_HDR(32, str)->len;\
                break;\
            case CXMLSTRING_HDR64:  \
                (out_len) = CXMLS_ACCESS_HDR(64, str)->len;\
                break;\
            default:\
                (out_len) = 0;\
        }\
    }while(0)

#define CXMLS_GET_SIZE(str, out_size)\
    do{\
        switch(CXMLS_GET_HDRTYPE(str)){\
            case CXMLSTRING_HDR8:  \
                (out_size) = CXMLS_ACCESS_HDR(8, str)->len;\
                break;\
            case CXMLSTRING_HDR16:  \
                (out_size) = CXMLS_ACCESS_HDR(16, str)->len;\
                break;\
            case CXMLSTRING_HDR32:  \
                (out_size) = CXMLS_ACCESS_HDR(32, str)->len;\
                break;\
            case CXMLSTRING_HDR64:  \
                (out_size) = CXMLS_ACCESS_HDR(64, str)->len;\
                break;\
            default:\
                (out_size) = 0;\
        }\
    }while(0)

#define CXMLS_SET_LEN(str, newlen)                                          \
    do{                                                                         \
        switch(CXMLS_GET_HDRTYPE(str)){                                    \
            case CXMLSTRING_HDR8:                                               \
                CXMLS_ACCESS_HDR(8, str)->len = (uint8_t)(newlen);         \
                break;                                                          \
            case CXMLSTRING_HDR16:                                              \
                CXMLS_ACCESS_HDR(16, str)->len = (uint16_t)(newlen);       \
                break;                                                          \
            case CXMLSTRING_HDR32:                                              \
                CXMLS_ACCESS_HDR(32, str)->len = (uint32_t)(newlen);       \
                break;                                                          \
            case CXMLSTRING_HDR64:                                              \
                CXMLS_ACCESS_HDR(64, str)->len = newlen;                   \
                break;                                                          \
        }                                                                       \
    }while(0)

#define CXMLS_SET_SIZE(str, newsize)                                        \
    do{                                                                         \
        switch(CXMLS_GET_HDRTYPE(str)){                                    \
            case CXMLSTRING_HDR8:                                               \
                CXMLS_ACCESS_HDR(8, str)->size = (uint8_t)(newsize);       \
                break;                                                          \
            case CXMLSTRING_HDR16:                                              \
                CXMLS_ACCESS_HDR(16, str)->size = (uint16_t)(newsize);     \
                break;                                                          \
            case CXMLSTRING_HDR32:                                              \
                CXMLS_ACCESS_HDR(32, str)->size = (uint32_t)(newsize);     \
                break;                                                          \
            case CXMLSTRING_HDR64:                                              \
                CXMLS_ACCESS_HDR(64, str)->size = newsize;                 \
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

static inline size_t getHdrSize(enum CXmlStringHdrType hdrType)
{
    switch(hdrType){
        case CXMLSTRING_HDR8:  return sizeof(struct CXmlStringHdr8);
        case CXMLSTRING_HDR16: return sizeof(struct CXmlStringHdr16);
        case CXMLSTRING_HDR32: return sizeof(struct CXmlStringHdr32);
        case CXMLSTRING_HDR64: return sizeof(struct CXmlStringHdr64);
        default:               return 0;
    }
}

static inline enum CXmlStringHdrType getHdrType(size_t size)
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


static inline CXmlString *allocWithHdr(size_t size)
{
    enum CXmlStringHdrType hdrType = getHdrType(size);
    size_t hdrSize = getHdrSize(hdrType);
    CXmlString *ptr = malloc(size+hdrSize+CXMLS_HDR_MARKER_SIZE);
    if(ptr == NULL){
        cxmlerrno = CXML_EALLOC;
        return NULL;
    }
    *(CXMLS_HDR_MARKER*)(ptr+hdrSize) = hdrType;
    return ptr+hdrSize+CXMLS_HDR_MARKER_SIZE;
}

static inline void freeAlloced(CXmlString *str)
{
    switch(CXMLS_GET_HDRTYPE(str)) {
        case CXMLSTRING_HDR8:
            free(CXMLS_ACCESS_HDR(8, str));
            break;
        case CXMLSTRING_HDR16:
            free(CXMLS_ACCESS_HDR(16, str));
            break;
        case CXMLSTRING_HDR32:
            free(CXMLS_ACCESS_HDR(32, str));
            break;
        case CXMLSTRING_HDR64:
            free(CXMLS_ACCESS_HDR(64, str));
            break;
    }
}

static inline CXmlString *reallocWithHdr(CXmlString *str, size_t size)
{
    size_t origLen = 0;
    if(CXML_NOTNULL(str)){
        CXMLS_GET_LEN(str, origLen);
    }
    enum CXmlStringHdrType hdrType = getHdrType(size);
    size_t hdrSize = getHdrSize(hdrType);

    switch(CXMLS_GET_HDRTYPE(str)) {
        case CXMLSTRING_HDR8:
            str = realloc(CXMLS_ACCESS_HDR(8, str), size+hdrSize+CXMLS_HDR_MARKER_SIZE);
            break;
        case CXMLSTRING_HDR16:
            str = realloc(CXMLS_ACCESS_HDR(16, str), size+hdrSize+CXMLS_HDR_MARKER_SIZE);
            break;
        case CXMLSTRING_HDR32:
            str = realloc(CXMLS_ACCESS_HDR(32, str), size+hdrSize+CXMLS_HDR_MARKER_SIZE);
            break;
        case CXMLSTRING_HDR64:
            str = realloc(CXMLS_ACCESS_HDR(64, str), size+hdrSize+CXMLS_HDR_MARKER_SIZE);
            break;
    }
    if(str == NULL){
        cxmlerrno = CXML_EALLOC;
        return NULL;
    }
    *(CXMLS_HDR_MARKER*)(str+hdrSize) = hdrType;
    CXMLS_SET_LEN(str, origLen);
    return str+hdrSize+CXMLS_HDR_MARKER_SIZE;
}

CXmlString *CXmlString_new(const char *str)
{
    return CXmlString_newLen(str, strlen(str));
}

CXmlString *CXmlString_newLen(const char *str, size_t len)
{
    CXML_RETNULL_IFNULL(str);
    CXmlString *newstr = allocWithHdr(len);
    if(newstr == NULL) return NULL;
    memset(newstr, '\0', len);
    memcpy(newstr, str, len);
    CXMLS_APPLY_TERMINATOR(newstr, len);
    CXMLS_SET_LEN(newstr, strlen(newstr));
    CXMLS_SET_SIZE(newstr, len);
    return newstr;
}

CXmlString *CXmlString_empty()
{
    return CXmlString_newLen("", 0);
}

size_t CXmlString_len(const CXmlString *str)
{
    size_t len;
    CXMLS_GET_LEN(str, len);
    return len;
}

int CXmlString_isEmpty(const CXmlString *str)
{
    size_t len;
    CXMLS_GET_LEN(str, len);
    return len == 0;
}

CXmlString *CXmlString_dup(const CXmlString *str)
{
    size_t len;
    CXMLS_GET_LEN(str, len);
    return CXmlString_newLen(str, len);
}

CXmlString *CXmlString_trim(const CXmlString *str)
{
    NOT_IMPLEMENTED //TODO Implement
}

CXmlString *CXmlString_copy(CXmlString **dest, const CXmlString *src)
{
    CXML_RETNULL_IFNULL(src);
    size_t srclen;
    CXMLS_GET_LEN(src, srclen);
    return CXmlString_ncopy(dest, src, srclen);
}

CXmlString *CXmlString_ncopy(CXmlString **dest, const CXmlString *src, size_t len)
{
    CXML_RESET_ERRNO__
    CXML_RETNULL_IFNULL(dest);
    CXML_RETNULL_IFNULL(src);
    size_t destSize;
    CXMLS_GET_SIZE(*dest, destSize);

    if(len > destSize){
        //Need to realloc *dest to fit

        enum CXmlStringHdrType newHdrType = getHdrType(len);
        CXmlString *tmp = reallocWithHdr(*dest, len);
        if(tmp == NULL) return NULL;
        *dest = tmp;
    }

    memcpy(*dest, src, len);
    CXMLS_SET_SIZE(*dest, destSize > len ? destSize : len);
    CXMLS_SET_LEN(*dest, len);
    return *dest;
}

CXmlString *CXmlString_move(CXmlString **dest, const CXmlString *src)
{
    CXML_RESET_ERRNO__
    CXML_RETNULL_IFNULL(src);
    size_t srclen;
    CXMLS_GET_LEN(src, srclen);
    return CXmlString_nmove(dest, src, srclen);
}

CXmlString *CXmlString_nmove(CXmlString **dest, const CXmlString *src, size_t len)
{
    CXML_RESET_ERRNO__
    CXML_RETNULL_IFNULL(dest);
    CXML_RETNULL_IFNULL(src);
    char buf[len];
    memcpy(buf, src, len);
    return CXmlString_ncopy(dest, buf, len);
}

CXmlString *CXmlString_cpcopy(CXmlString **dest, const char *src)
{
    CXML_RESET_ERRNO__
    return CXmlString_copy(dest, src);
}

CXmlString *CXmlString_cpncopy(CXmlString **dest, const char *src, size_t len)
{
    CXML_RESET_ERRNO__
    return CXmlString_ncopy(dest, src, len);
}

CXmlString *CXmlString_substr(const CXmlString *str, size_t start)
{
    CXML_RESET_ERRNO__
    CXML_RETNULL_IFNULL(str);
    size_t len;
    CXMLS_GET_LEN(str, len);
    if(start >= len){
        cxmlerrno = CXML_EOUTBOUNDS;
        return NULL;
    }
    CXmlString *substr = CXmlString_new(str+start);
    return substr;
}

CXmlString *CXmlString_substrLen(const CXmlString *str, size_t start, size_t len)
{
    CXML_RESET_ERRNO__
    CXML_RETNULL_IFNULL(str);
    size_t slen;
    CXMLS_GET_LEN(str, slen);
    if(start >= slen || start+len > slen){
        cxmlerrno = CXML_EOUTBOUNDS;
        return NULL;
    }
    CXmlString *substr = CXmlString_newLen(str+start, len);
    strncpy(substr, str+start, len);
    CXMLS_SET_LEN(substr, len);
    return substr;
}

int CXmlString_comp(const CXmlString *str1, const CXmlString *str2)
{
    CXML_RESET_ERRNO__
    CXML_RETNULL_IFNULL(str1);
    CXML_RETNULL_IFNULL(str2);
    return str1 == str2 ? 0 : strcmp(str1, str2);
}

int CXmlString_contains(const CXmlString *str, const CXmlString *sterm)
{
    CXML_RESET_ERRNO__
    return CXmlString_indexOf(str, sterm, NULL);
}

int CXmlString_cpcontains(const CXmlString *str, const char *sterm)
{
    CXML_RESET_ERRNO__
    return CXmlString_indexOf(str, sterm, NULL);
}

int CXmlString_indexOf(const CXmlString *str, const CXmlString *sterm, size_t *index)
{
    CXML_RESET_ERRNO__
    CXML_RETNULL_IFNULL(str);
    CXML_RETNULL_IFNULL(sterm);

    char *result = strstr(str, "abc");
    if(result == NULL) return 0;

    if(index != NULL) *index = result - str;
    return 1;
}

int CXmlString_cpindexOf(const CXmlString *str, const char *sterm, size_t *index)
{
    CXML_RESET_ERRNO__
    return CXmlString_indexOf(str, sterm, index);
}