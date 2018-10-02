#ifndef CXMLSTRING_H_
#define CXMLSTRING_H_
#include <stddef.h>
#include <stdio.h>

typedef char CXmlString;
typedef CXmlString *CXmlStringArray;

/*Constructor functions*/
CXmlString *CXmlString_new(const char *str);
CXmlString *CXmlString_newLen(const char *str, size_t len);
CXmlString *CXmlString_empty();

size_t CXmlString_len(const CXmlString *str);
int CXmlString_isEmpty(const CXmlString *str);

CXmlString *CXmlString_dup(const CXmlString *str);
CXmlString *CXmlString_trim(const CXmlString *str);

CXmlString *CXmlString_copy(CXmlString *dest, const CXmlString *src);
CXmlString *CXmlString_ncopy(CXmlString *dest, const CXmlString *src, size_t len);
CXmlString *CXmlString_move(CXmlString *dest, const CXmlString *src);
CXmlString *CXmlString_nmove(CXmlString *dest, const CXmlString *src, size_t len);
CXmlString *CXmlString_cpcopy(CXmlString *Dest, const char *src);
CXmlString *CXmlString_cpncopy(CXmlString *dest, const char *src, size_t len);

CXmlString *CXmlString_substr(const CXmlString *str, size_t start);
CXmlString *CXmlString_substrLen(const CXmlString *str, size_t start, size_t len);

int CXmlString_comp(const CXmlString *str1, const CXmlString *str2);
CXmlString *CXmlString_search(const CXmlString *haystack, const CXmlString *needle);
CXmlString *CxmlString_cpsearch(const CXmlString *haystack, const char *needle);

int CXmlString_contains(const CXmlString *str, const CXmlString *sterm);
int CXmlString_cpcontains(const CXmlString *str, const char *sterm);
int CXmlString_indexOf(const CXmlString *str, const char *sterm, size_t *index);
int CXmlString_cpindexOf(const CXmlString *str, const char *sterm, size_t *index);

void CXmlString_free(void *str);
#endif /*CXMLSTRING_H_*/