//#define ECT_ABORT_ON_FAIL
#include "ectest.h"

ECT_DECLARE_BEFORE_MODULE(ECT_NO_SETUP);
ECT_DECLARE_AFTER_MODULE(ECT_NO_TEARDOWN);
ECT_DECLARE_BEFORE_TEST(ECT_NO_SETUP);
ECT_DECLARE_AFTER_TEST(ECT_NO_TEARDOWN);
ECT_DECLARE_TESTS(
    SuccessFunc,
    SkipFunc,
    FailFunc
);
ECT_EXPORT_MODULE(TestModule)


ECT_TEST(SuccessFunc)
{
    ECT_SUCCESS();
}

ECT_TEST(SkipFunc)
{
    ECT_SKIP();
}

ECT_TEST(FailFunc)
{
    ECT_FAIL("Testing failure");
}
