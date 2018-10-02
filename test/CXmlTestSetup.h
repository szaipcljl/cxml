#ifndef _CXMLTESTSETUP_H__
#define _CXMLTESTSETUP_H__
#include <stdio.h>

#define stringize__(x) #x
#define stringize2__(x) stringize__(x)
#define TEST_MODULE_STRING__ stringize2__(TEST_MODULE)

#define TEST_CONC__(x, y) x##_##y
#define TEST_CONC2__(x, y) TEST_CONC__(x,y)

#define TEST_FUNC(funcname) int TEST_CONC2__(TEST_MODULE,funcname)()

#define TEST_RUNNER() \
    int TEST_MODULE##_##run()\
    {\
        unsigned failedtests_ = 0;\
        unsigned tests_ = 0;

#define END_TEST_RUNNER\
            printf("Module %s: Total %d | Success: %d | Failed: %d\n",\
                TEST_MODULE_STRING__, \
                tests_, \
                tests_-failedtests_,\
                failedtests_);\
        return !failedtests_ ? 0 : -1;\
    }

#ifdef CXML_TEST_ABORT_ON_FAIL
#   define RUN_TEST_FUNC(funcname)\
        do{\
            tests_++;\
            int res = Test_##funcname();\
            if(!res){\
                failedtests_++;\
                printf("Module %s: Test failed, aborting test run\n",\
                       TEST_MODULE_STRING__);\
                return -1;\
            }\
        }while(0)
#else
#   define RUN_TEST_FUNC(funcname)\
        do{\
            tests_++;\
            int res = TEST_CONC2__(TEST_MODULE,funcname)();\
            if(!res){\
                failedtests_++;\
            }\
        }while(0)
#endif


#define TEST_FAIL(fmt, ...) \
    do{\
        printf("[FAILED] %s | "fmt"\n", __VA_ARGS__);\
        return 0;\
    }while(0)

#ifdef CXML_VERBOSE_TEST
#   define TEST_SUCCESS() \
        do{\
            printf(,"[SUCCESS] %s\n", __func__);\
            return 1;\
        }while(0)
#else
#   define TEST_SUCCESS() return 1;
#endif

#define TEST_ASSERTF(cond, failfmt, ...)            \
    do{                                         \
        if(!(cond)){                            \
            TEST_FAIL(failfmt, __VA_ARGS__);        \
        }                                       \
    }while(0)

#define TEST_ASSERT(cond, failmsg) TEST_ASSERTF(cond, "%s", failmsg)

#endif // _CXMLTESTSETUP_H__