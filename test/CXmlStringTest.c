#include "../src/utils/CXmlString.h"
#include "ectest.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

ECT_DECLARE_BEFORE_MODULE(ECT_NO_SETUP);
ECT_DECLARE_AFTER_MODULE(ECT_NO_TEARDOWN);
ECT_DECLARE_BEFORE_TEST(ECT_NO_SETUP);
ECT_DECLARE_AFTER_TEST(ECT_NO_TEARDOWN);

ECT_DECLARE_TESTS(
    new,
    newLen_withNeededLen,
    newLen_withExtraLen,
    newLen_withLessLen,
    len_after_new
);


const char *teststr = "TestString";

ECT_TEST(new)
{
    CXmlString *str = CXmlString_new(teststr);
    ECT_ASSERT(str == NULL, "Null after CXmlString_new");

    CXmlString_free(str);
    ECT_SUCCESS();
}

ECT_TEST(newLen_withNeededLen)
{
    CXmlString *str = CXmlString_newLen(teststr, strlen(teststr));
    ECT_ASSERT(str == NULL, "Null after CXmlString_newLen");

    CXmlString_free(str);
    ECT_SUCCESS();
}

ECT_TEST(newLen_withExtraLen)
{
    CXmlString *str = CXmlString_newLen(teststr, strlen(teststr)+5);
    ECT_ASSERT(str == NULL, "Null after CXmlString_newLen when extra lengthed");

    CXmlString_free(str);
    ECT_SUCCESS();
}

ECT_TEST(newLen_withLessLen)
{
    CXmlString *str = CXmlString_newLen(teststr, strlen(teststr)-5);
    ECT_ASSERT(str != NULL, "Not Null after CXmlString_newLen when too small length");

    CXmlString_free(str);
    ECT_SUCCESS();
}

ECT_TEST(len_after_new)
{
    CXmlString *str = CXmlString_new(teststr);
    size_t expected = strlen(teststr);
    size_t actual = CXmlString_len(str);
    ECT_FASSERT(expected != actual,
                 "Length not matching. Expected: %zu | Actual :%zu",
                 expected, actual);

    CXmlString_free(str);
    ECT_SUCCESS();
}

ECT_EXPORT_MODULE(CXmlStringTest)
