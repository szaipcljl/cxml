#ifndef _CXMLTESTSETUP_H__
#define _CXMLTESTSETUP_H__
#include <stdio.h>

#define TEST_MODULE_BEGIN TEST_MODULE_BEGIN__
#define TEST_MODULE_BEGIN__ TEST_DECLFUNC__(void,){}
#define TEST_MODULE_END TEST_MODULE_END__

#define TEST_FOREACH__(item, array) \
    for(int array##keep__ = 1, \
            array##count__ = 0,\
            array##size__ = sizeof (array) / sizeof *(array); \
        array##keep__ && array##count__ != array##size__; \
        array##keep__ = !array##keep__, array##count__++) \
      for(item = (array)[array##count__]; array##keep__; array##keep__ = !array##keep__)

#define TEST_MODULE_END__ \
    int TEST_CONCWU__(TEST_MODULE, run)(void)\
    {\
            /*Run module setup functions*/\
            TEST_FOREACH__(TEST_SUTD_FUNC_PTR__(bmfunc), CXmlStringTest_BMARRAY){ bmfunc(); }\
            /*Loop over all test functions*/\
            TEST_FOREACH__(TEST_TEST_FUNC_PTR__(tfunc), CXmlStringTest_TARRAY){\
                /*Run test setup functions*/\
                TEST_FOREACH__(TEST_SUTD_FUNC_PTR__(btfunc), CXmlStringTest_BTARRAY){ btfunc(); }\
                /*Run test function*/\
                tfunc();\
                /*Run test teardown functions*/\
                TEST_FOREACH__(TEST_SUTD_FUNC_PTR__(atfunc), CXmlStringTest_BTARRAY){ atfunc(); }\
        }\
        /*Run module teardown functions*/\
        TEST_FOREACH__(TEST_SUTD_FUNC_PTR__(amfunc), CXmlStringTest_AMARRAY){ amfunc(); }\
        return 0;\
    }

/*** UTILITY MACROS ***/
#define TEST_PCONC__(x, y) x##y
#define TEST_CONC__(x, y) TEST_PCONC__(x, y)
#define TEST_PCONCWU__(x, y) x##_##y
#define TEST_CONCWU__(x, y) TEST_PCONCWU__(x,y)
#define TEST_ESC_PAR__(...) __VA_ARGS__
#define TEST_IF__1(t, f) t
#define TEST_IF__0(t, f) f
#define TEST_IF__(c, t, f) TEST_CONC__(TEST_IF__, c)(TEST_ESC_PAR__(t),TEST_ESC_PAR__(f))
#define TEST_PSTRINGIZE__(x) #x
#define TEST_STRINGIZE__(x) TEST_PSTRINGIZE__(x)

/*** LOOP BASE MACRO ***/
#define TEST_ARG_N__(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define TEST_RSEQ_N__() 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define TEST_NARG___(...) TEST_ARG_N__(__VA_ARGS__)
#define TEST_NARG__(...) TEST_NARG___(__VA_ARGS__, TEST_RSEQ_N__())

#define TEST_LOOP___(loop_pattern, N, ...) TEST_CONC__(loop_pattern, N)(__VA_ARGS__)

#define TEST_LOOP__(loop_pattern, ...) TEST_LOOP___(loop_pattern, TEST_NARG__(__VA_ARGS__), __VA_ARGS__)

/*** END LOOP BASE MACRO ***/
/*** END UTILITY MACROS ***/

/*** BASE TEST FUNCTION ARRAY DEFINES ***/
#define TEST_TEST_FUNC_PTR__(name) int(*name)(void)

#define TEST_TEST_FUNC_PTR_ARR__(modulename, name) \
    TEST_TEST_FUNC_PTR__(TEST_CONCWU__(modulename, name)[])

#define TEST_DECLFUNC__(type, name) static type TEST_CONCWU__(TEST_MODULE,name)(void)

#define TEST_SUTD_FUNC_PTR__(name) void(*name)(void)
#define TEST_SUTD_FUNC_PTR_ARR__(modulename, name)\
    TEST_SUTD_FUNC_PTR__(TEST_CONCWU__(modulename, name)[])

#define TEST_SIZEOFARRAY__(name) \
    (sizeof(name)/sizeof(*(name)))

#define TEST_DECL_ARRSIZE__(modulename, arrname)\
    size_t TEST_CONCWU__(modulename,TEST_CONCWU__(arrname,count))

#define TEST_MODULE_SIZEOFARRAY__(name) TEST_SIZEOFARRAY__(TEST_CONCWU__(TEST_MODULE, name))

/*** END BASE TEST FUNCTION ARRAY DEFINES***/

/*** MODULE TEST FUNCTION ARRAY DEFINES***/
#define TEST_MODULE_TARRAY__(modulename) TEST_TEST_FUNC_PTR_ARR__(modulename, TARRAY)
#define TEST_MODULE_TARRAYSIZE__(modulename) TEST_DECL_ARRSIZE__(modulename, TARRAY)

#define TEST_MODULE_BTARRAY__(modulename) TEST_SUTD_FUNC_PTR_ARR__(modulename, BTARRAY)
#define TEST_MODULE_BTARRAYSIZE__(modulename)  TEST_DECL_ARRSIZE__(modulename, BTARRAY)

#define TEST_MODULE_ATARRAY__(modulename) TEST_SUTD_FUNC_PTR_ARR__(modulename, ATARRAY)
#define TEST_MODULE_ATARRAYSIZE__(modulename)  TEST_DECL_ARRSIZE__(modulename, ATARRAY)

#define TEST_MODULE_BMARRAY__(modulename) TEST_SUTD_FUNC_PTR_ARR__(modulename, BMARRAY)
#define TEST_MODULE_BMARRAYSIZE__(modulename)  TEST_DECL_ARRSIZE__(modulename, BMARRAY)

#define TEST_MODULE_AMARRAY__(modulename) TEST_SUTD_FUNC_PTR_ARR__(modulename, AMARRAY)
#define TEST_MODULE_AMARRAYSIZE__(modulename)  TEST_DECL_ARRSIZE__(modulename, AMARRAY)
/*** END MODULE TEST FUNCTION ARRAY DEFINES***/

/*** MODULE EXPORT ***/

#define TEST_IMPORT_MODULE__(modulename, export)\
    extern TEST_MODULE_##export##__(modulename);\
    extern TEST_MODULE_##export##SIZE__(modulename)

#define TEST_IMPORT_MODULE_RUN__(modulename)\
    extern int TEST_CONCWU__(modulename, run)(void)

#define TEST_IMPORT_MODULE(modulename)\
    TEST_IMPORT_MODULE_RUN__(modulename);\
    TEST_IMPORT_MODULE__(modulename, TARRAY);\
    TEST_IMPORT_MODULE__(modulename, BTARRAY);\
    TEST_IMPORT_MODULE__(modulename, ATARRAY);\
    TEST_IMPORT_MODULE__(modulename, BMARRAY);\
    TEST_IMPORT_MODULE__(modulename, AMARRAY)

#define TEST_EXPORT_FUNCS__(arrname, type, ...)\
    TEST_DECLEXPORT_FUNC__(type, __VA_ARGS__)\
    TEST_MODULE_##arrname##__(TEST_MODULE) = {\
    TEST_LOOP__(TEST_EXPORT_FUNC__, __VA_ARGS__)\
    };\
    TEST_MODULE_##arrname##SIZE__(TEST_MODULE) = TEST_MODULE_SIZEOFARRAY__(arrname)


#define TEST_EXPORT_TESTS(...) TEST_EXPORT_FUNCS__(TARRAY, int, __VA_ARGS__)
#define TEST_EXPORT_MODULE_SETUPS(...) TEST_EXPORT_FUNCS__(BMARRAY, void, __VA_ARGS__)
#define TEST_EXPORT_MODULE_TEARDOWNS(...) TEST_EXPORT_FUNCS__(AMARRAY, void, __VA_ARGS__)
#define TEST_EXPORT_TEST_SETUPS(...) TEST_EXPORT_FUNCS__(BTARRAY, void, __VA_ARGS__)
#define TEST_EXPORT_TEST_TEARDOWNS(...) TEST_EXPORT_FUNCS__(ATARRAY, void, __VA_ARGS__)

/*** EXPORT LOOPS ***/
#define TEST_EXPORT_FUNC__1(arg) &TEST_CONCWU__(TEST_MODULE,arg)
#define TEST_EXPORT_FUNC__2(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__1(__VA_ARGS__)
#define TEST_EXPORT_FUNC__3(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__2(__VA_ARGS__)
#define TEST_EXPORT_FUNC__4(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__3(__VA_ARGS__)
#define TEST_EXPORT_FUNC__5(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__4(__VA_ARGS__)
#define TEST_EXPORT_FUNC__6(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__5(__VA_ARGS__)
#define TEST_EXPORT_FUNC__7(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__6(__VA_ARGS__)
#define TEST_EXPORT_FUNC__8(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__7(__VA_ARGS__)
#define TEST_EXPORT_FUNC__9(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__8(__VA_ARGS__)
#define TEST_EXPORT_FUNC__10(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__9(__VA_ARGS__)
#define TEST_EXPORT_FUNC__11(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__10(__VA_ARGS__)
#define TEST_EXPORT_FUNC__12(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__11(__VA_ARGS__)
#define TEST_EXPORT_FUNC__13(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__12(__VA_ARGS__)
#define TEST_EXPORT_FUNC__14(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__13(__VA_ARGS__)
#define TEST_EXPORT_FUNC__15(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__14(__VA_ARGS__)
#define TEST_EXPORT_FUNC__16(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__15(__VA_ARGS__)
#define TEST_EXPORT_FUNC__17(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__16(__VA_ARGS__)
#define TEST_EXPORT_FUNC__18(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__17(__VA_ARGS__)
#define TEST_EXPORT_FUNC__19(arg, ...) &TEST_CONCWU__(TEST_MODULE,arg), TEST_EXPORT_FUNC__18(__VA_ARGS__)

#define TEST_DECLEXPORT_FUNC__1(type, arg) TEST_DECLFUNC__(type, arg);
#define TEST_DECLEXPORT_FUNC__2(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__1(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__3(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__2(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__4(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__3(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__5(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__4(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__6(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__5(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__7(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__6(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__8(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__7(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__9(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__8(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__10(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__9(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__11(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__10(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__12(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__11(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__13(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__12(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__14(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__13(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__15(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__14(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__16(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__15(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__17(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__16(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__18(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__17(type, __VA_ARGS__)
#define TEST_DECLEXPORT_FUNC__19(type, arg, ...) TEST_DECLFUNC__(type, arg); TEST_DECLEXPORT_FUNC__18(type, __VA_ARGS__)

#define TEST_DECLEXPORT_FUNC__(type, ...) \
    TEST_LOOP___(TEST_DECLEXPORT_FUNC__, TEST_NARG__(__VA_ARGS__), type, __VA_ARGS__)
/*** END EXPORT LOOPS ***/

/*** END MODULE EXPORT ***/

        
#define TEST_TEST(funcname) TEST_DECLFUNC__(int, funcname)
#define TEST_TEST_SETUP(funcname) TEST_DECLFUNC__(void, funcname)
#define TEST_TEST_TEARDOWN(funcname) TEST_DECLFUNC__(void, funcname)
#define TEST_MODULE_SETUP(funcname) TEST_DECLFUNC__(void, funcname)
#define TEST_MODULE_TEARDOWN(funcname) TEST_DECLFUNC__(void, funcname)
/*** TEST FAIL/SUCCESS ***/

#define TEST_FAIL(fmt, ...) \
    do{\
        printf("[FAILED] %s | "fmt"\n", __func__, __VA_ARGS__);\
        return 0;\
    }while(0)

#ifdef CXML_VERBOSE_TESTz
#   define TEST_SUCCESS() \
        do{\
            printf(,"[SUCCESS] %s\n", __func__);\
            return 1;\
        }while(0)
#else
#   define TEST_SUCCESS() return 1;
#endif
/*** END TEST FAIL/SUCCESS ***/

/*** ASSERT ***/
#define TEST_ASSERTF(cond, failfmt, ...)            \
    do{                                         \
        if(!(cond)){                            \
            TEST_FAIL(failfmt, __VA_ARGS__);        \
        }                                       \
    }while(0)

#define TEST_ASSERT(cond, failmsg) TEST_ASSERTF(cond, "%s", failmsg)
/*** END ASSERT ***/

#endif // _CXMLTESTSETUP_H__