#include "CXmlStringTest.h"
#include "../src/utils/CXmlString.h"
#include "CXmlTestSetup.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*TODO FOR TEST FRAMEWORK
 *
 * 1. define TEST_RUN_MODULE(modulename)
 * 2. implement ABORT_ON_FAIL
 * 3. increase number of TEST_ARG_N__ and TEST_RSEQ_N__ instances.
 * 4. increase number of TEST_EXPORT_FUNC__ and TEST_DECLEXPORT_FUNC__ iterations.
 * 5. restructure CXmlTestSetup.
 * 6. split CXmlTestSetup to one for internal macros and one for public macros.
 * 7. Rename CXmlTestSetup to own file id, it will be exported to its own project.
 * 8. Rename prefix from TEST to something to reflect new file id.
 * 9. Write quick doc for how to setup testing modules.
 */

#define TEST_MODULE CXmlStringTest
TEST_MODULE_BEGIN

TEST_EXPORT_MODULE_SETUPS();
TEST_EXPORT_MODULE_TEARDOWNS();
TEST_EXPORT_TEST_SETUPS();
TEST_EXPORT_TEST_TEARDOWNS();

TEST_EXPORT_TESTS(
    new,
    newLen_withNeededLen,
    newLen_withExtraLen,
    newLen_withLessLen,
    len_after_new
);


const char *teststr = "TestString";

TEST_TEST(new)
{
    CXmlString *str = CXmlString_new(teststr);
    TEST_ASSERT(str == NULL, "Null after CXmlString_new");

    CXmlString_free(str);
    TEST_SUCCESS();
}

TEST_TEST(newLen_withNeededLen)
{
    CXmlString *str = CXmlString_newLen(teststr, strlen(teststr));
    TEST_ASSERT(str == NULL, "Null after CXmlString_newLen");

    CXmlString_free(str);
    TEST_SUCCESS();
}

TEST_TEST(newLen_withExtraLen)
{
    CXmlString *str = CXmlString_newLen(teststr, strlen(teststr)+5);
    TEST_ASSERT(str == NULL, "Null after CXmlString_newLen when extra lengthed");

    CXmlString_free(str);
    TEST_SUCCESS();
}

TEST_TEST(newLen_withLessLen)
{
    CXmlString *str = CXmlString_newLen(teststr, strlen(teststr)-5);
    TEST_ASSERT(str != NULL, "Not Null after CXmlString_newLen when too small length");

    CXmlString_free(str);
    TEST_SUCCESS();
}

TEST_TEST(len_after_new)
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

TEST_MODULE_END
#undef TEST_MODULE