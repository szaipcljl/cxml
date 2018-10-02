#include "CXmlStringTest.h"
#include "../src/utils/CXmlString.h"
#include "CXmlTestSetup.h"
#include <string.h>
#include <stdio.h>

#define TEST_MODULE CXmlStringTest

/*
CXmlString *CXmlString_new(const char *str);
CXmlString *CXmlString_newLen(const char *str, size_t len);
CXmlString *CXmlString_empty();

size_t CXmlString_len(const CXmlString *str);
int CXmlString_isEmpty(const CXmlString *str);

CXmlString *CXmlString_dup(const CXmlString *str);
CXmlString *CXmlString_trim(const CXmlString *str);

CXmlString *CXmlString_copy(CXmlString **dest, const CXmlString *src);
CXmlString *CXmlString_ncopy(CXmlString **dest, const CXmlString *src, size_t len);
CXmlString *CXmlString_move(CXmlString **dest, const CXmlString *src);
CXmlString *CXmlString_nmove(CXmlString **dest, const CXmlString *src, size_t len);
CXmlString *CXmlString_cpcopy(CXmlString **dest, const char *src);
CXmlString *CXmlString_cpncopy(CXmlString **dest, const char *src, size_t len);

CXmlString *CXmlString_substr(const CXmlString *str, size_t start);
CXmlString *CXmlString_substrLen(const CXmlString *str, size_t start, size_t len);

int CXmlString_comp(const CXmlString *str1, const CXmlString *str2);
CXmlString *CXmlString_search(const CXmlString *haystack, const CXmlString *needle);
CXmlString *CxmlString_cpsearch(const CXmlString *haystack, const char *needle);

int CXmlString_contains(const CXmlString *str, const CXmlString *sterm);
int CXmlString_cpcontains(const CXmlString *str, const char *sterm);
int CXmlString_indexOf(const CXmlString *str, const CXmlString *sterm, size_t *index);
int CXmlString_cpindexOf(const CXmlString *str, const char *sterm, size_t *index);

void CXmlString_free(void *str);
*/

const char *teststr = "TestString";

TEST_FUNC(new)
{
    CXmlString *str = CXmlString_new(teststr);
    TEST_ASSERT(str == NULL, "Null after CXmlString_new");

    CXmlString_free(str);
    TEST_SUCCESS();
}

TEST_FUNC(newLen_withNeededLen)
{
    CXmlString *str = CXmlString_newLen(teststr, strlen(teststr));
    TEST_ASSERT(str == NULL, "Null after CXmlString_newLen");

    CXmlString_free(str);
    TEST_SUCCESS();
}

TEST_FUNC(newLen_withExtraLen)
{
    CXmlString *str = CXmlString_newLen(teststr, strlen(teststr)+5);
    TEST_ASSERT(str == NULL, "Null after CXmlString_newLen when extra lengthed");

    CXmlString_free(str);
    TEST_SUCCESS();
}

TEST_FUNC(newLen_withLessLen)
{
    CXmlString *str = CXmlString_newLen(teststr, strlen(teststr)-5);
    TEST_ASSERT(str != NULL, "Not Null after CXmlString_newLen when too small length");

    CXmlString_free(str);
    TEST_SUCCESS();
}

TEST_FUNC(len_after_new)
{
    CXmlString *str = CXmlString_new(teststr);
    size_t expected = strlen(teststr);
    size_t actual = CXmlString_len(str);
    TEST_ASSERTF(expected != actual,
                 "Length not matching. Expected: %zu | Actual :%zu",
                 expected, actual);

    CXmlString_free(str);
    TEST_SUCCESS();
}

//#define stringize(x) #x
TEST_RUNNER()
{
    RUN_TEST_FUNC(new);
}
END_TEST_RUNNER

#undef TEST_MODULE