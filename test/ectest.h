#ifndef _ECTEST_H__
#define _ECTEST_H__

/*TODO FOR TEST FRAMEWORK
 *
 * 1. increase number of TEST_ARG_N__ and TEST_RSEQ_N__ instances.
 * 2. increase number of TEST_EXPORT_FUNC__ and TEST_DECLEXPORT_FUNC__ iterations.
 * 3. Document public interface.
 * 4. Write quick doc for how to setup testing modules. (readme)
 * 5. Setup CI
 */

#include <stdio.h>
/**
 * @file ectest.h
 * @brief Contains everything needed for use of the ECTest testing framework.
 *
 * This files contains all macros, data structures and functionality that make up the ECTest testing framework.
 * To use ECTest, include this file and utilize the macros defined under the public section.
 *
 * Note: Do not use anything under the internal section, those are for internal usage of the ECTest implementation.
 * Use of internal macros, functions or data structures within test modules and runners are considered undefined
 * behaviour in the context of the ECTest testing framework. Functionality under the internal section may be changed
 * at any time without notification, unless the changes affect the public interfaces.
 */

/* COMPILE DEFINE INFO
 *
 * ETC_ABORT_ON_FAIL
 * To abort testing upon test case failure, compile with -DETC_ABORT_ON_FAIL=1
 *
 * ETC_LOG_LEVEL
 * Choose what level of test completion logging should be done (success/skipped/failed).
 * Compile with -DETC_LOG_LEVEL=3 for all (default)
 * Compile with -DETC_LOG_LEVEL=2 for failed and skipped only.
 * Compile with -DETC_LOG_LEVEL=1 for failed only.
 * Compile with -DETC_LOG_LEVEL=0 for only module run notifications and summaries.
 */

/* PUBLIC
 *
 * This section contains macros for public use within test modules and runners.
 *
 * Some macros have a variant that is provides a @ref printf style format string, and takes a variable amount of
 * arguments, instead of a fixed message. These macros has the same name as their fixed message equivalents, with the
 * exception that they start with ECT_F<rest of name> instead of ECT_<rest of name>. These macros are not documented
 * as they follow the same behaviour as their fixed message equivalents.
 *
 * Only use macros within this section when writing tests, the usage of macros/functions/data structures under the
 * internal section is considered undefined behaviour in the context of using the ECTest testing framework.
 */

#define ECT_IMPORT_MODULE(modulename) ECT_IMPORT_MODULE__(modulename)
#define ECT_IMPORT_MODULES(...) ECT_IMPORT_MODULES__(__VA_ARGS__)
#define ECT_RUN_MODULE(modulename) ECT_RUN_MODULE__(modulename)

#define ECT_EXPORT_MODULE(modulename) ECT_EXPORT_MODULE__(modulename)

/**
 * @defgroup TESTDECL Test Function Declaration Macros
 * @{
 */
#define ECT_DECLARE_TESTS(...) ECT_DECLARE_TESTS__(__VA_ARGS__)
#define ECT_DECLARE_BEFORE_MODULE(...) ECT_DECLARE_BEFORE_MODULE__(__VA_ARGS__)
#define ECT_DECLARE_AFTER_MODULE(...) ECT_DECLARE_AFTER_MODULE__(__VA_ARGS__)
#define ECT_DECLARE_BEFORE_TEST(...) ECT_DECLARE_BEFORE_TEST__(__VA_ARGS__)
#define ECT_DECLARE_AFTER_TEST(...) ECT_DECLARE_AFTER_TEST__(__VA_ARGS__)
/**@}*/

/**
 * @defgroup TESTDEF Test Function Definition Macros
 * @{
 */
/**
 * @def ECT_TEST(funcname)
 * @brief Defines a function suitable for use as an ECTest test case.
 * @param funcname The name of the function (Do not quote as string)
 */
#define ECT_TEST(funcname) ECT_TEST__(funcname)

/**
 * @def ECT_BEFORE_TEST(funcname)
 * @brief Defines a function suitable for use as an ECTest setup function to be ran before each test case.
 * @param funcname The name of the function (Do not quote as string)
 */
#define ECT_BEFORE_TEST(funcname) ECT_BEFORE_TEST__(funcname)

/**
 * @def ECT_AFTER_TEST(funcname)
 * @brief Defines a function suitable for use as an ECTest teardown function to be ran after each test case.
 * @param funcname The name of the function (Do not quote as string)
 */
#define ECT_AFTER_TEST(funcname) ECT_AFTER_TEST__(funcname)

/**
 * @def ECT_BEFORE_MODULE(funcname)
 * @brief Defines a function suitable for use as an ECTest setup function to be ran before any of the test cases in
 *        a test module.
 * @param funcname The name of the function (Do not quote as string)
 */
#define ECT_BEFORE_MODULE(funcname) ECT_BEFORE_MODULE__(funcname)

/**
 * @def ECT_AFTER_MODULE(funcname)
 * @brief Defines a function suitable for use as an ECTest teardown function to be ran after any of the test cases in
 *        a test module.
 * @param funcname The name of the function (Do not quote as string)
 */
#define ECT_AFTER_MODULE(funcname) ECT_AFTER_MODULE__(funcname)
/**@}*/

/**
 * @defgroup TESTRESULT Test Result Macros
 * @{
 */
/**
 * @def ECT_SUCCESS()
 * @brief Use to mark a test case as successful.
 *
 * Use to mark a test case as successful. A test case must always exit using either via asserts or ECT_SUCCESS,
 * ECT_FFAIL, ECT_FAIL or ECT_SKIP
 */
#define ECT_SUCCESS() ECT_SUCCESS__()

/**
 * @def ECT_FAIL()
 * @brief Use to mark a test case as failed, providing a message.
 *
 * Use to mark a test case as failed, with a provided message. A test case must always exit using either via asserts or
 * ECT_SUCCESS, ECT_FFAIL, ECT_FAIL or ECT_SKIP
 */
#define ECT_FAIL(msg) ECT_FAIL__(msg)
#define ECT_FFAIL(fmt, ...) ECT_FFAIL__(fmt, __VA_ARGS__)

/**
 * @def ECT_SKIP()
 * @brief Use to mark a test case as skipped.
 *
 * Use to mark a test case as skipped. A test case must always exit using either via asserts or ECT_SUCCESS,
 * ECT_FFAIL, ECT_FAIL or ECT_SKIP
 */
#define ECT_SKIP() ECT_SKIP__()
/**@}*/
/**
 * @defgroup ASSERT Test Assert Macros
 * @{
 */
/**
 * @def ECT_ASSERT(expr, msg)
 * @brief Evaluates the expression. If it evaluates to true, the test continues, else it fails as if @ref ECT_FAIL had
 * been called.
 * @param expr The expression to evaluate.
 * @param msg The message to print on failed assert.
 */
#define ECT_ASSERT(expr, msg) ECT_ASSERT__(expr, msg)
#define ECT_FASSERT(expr, fmt, ...) ECT_FASSERT__(expr, fmt, __VA_ARGS__)

/**
 * @def ECT_ASSERT_TRUE(expr, msg)
 * @brief Alias for @ref ECT_ASSERT.
 */
#define ECT_ASSERT_TRUE(expr, msg) ECT_ASSERT(expr, msg)
#define ECT_FASSERT_TRUE(expr, fmt, ...) ECT_FASSERT(expr, fmt, __VA_ARGS__)

/**
 * @def ECT_ASSERT_FALSE(expr, msg)
 * @brief Evaluates the expression. If it evaluates to false, the test continues, else it fails as if @ref ECT_FAIL had
 * been called.
 * @param expr The expression to evaluate.
 * @param msg The message to print on failed assert.
 */
#define ECT_ASSERT_FALSE(expr, msg) ECT_ASSERT(!(expr), msg)
#define ECT_FASSERT_FALSE(expr, fmt, ...) ECT_FASSERT(!(expr), fmt, __VA_ARGS__)

/**
 * @def ECT_ASSERT_CONTAINS(array, item, cmpfunc, msg)
 * @brief Checks whether the array contains the item using the compare function. If not, it fails the test case and
 * prints the message specified.
 * @param array The array to check.
 * @param item The item to look for.
 * @param cmpfunc The comparator function. Must be a function that takes two items of the type of @item and returns a
 *                integer. The function should return 1 if the items compare equal; else 0.
 * @param msg The message to print on failed assert.
 */
//TODO IMPLEMENT
#define ECT_ASSERT_CONTAINS(array, item, cmpfunc, msg)
#define ECT_FASSERT_CONTAINS(array, item, cmpfunc, fmt, ...)

/**
 * @def ECT_ASSERT_NOT_CONTAINS(array, item, cmpfunc, msg)
 * @brief Checks whether the array contains the item using the compare function. If it does, it fails the test case and
 * prints the message specified.
 * @param array The array to check.
 * @param item The item to look for.
 * @param cmpfunc The comparator function. Must be a function that takes two items of the type of @item and returns a
 *                integer. The function should return 1 if the items compare equal; else 0.
 * @param msg The message to print on failed assert.
 */
//TODO IMPLEMENT
#define ECT_ASSERT_NOT_CONTAINS(array, item, cmpfunc, msg)
#define ECT_FASSERT_NOT_CONTAINS(array, item, cmpfunc, fmt, ...)

/**
 * @def ECT_ASSERT_CONTAINSN(array, item, noccurence, cmpfunc, msg)
 * @brief Checks whether the array contains the item using the compare function, the same way that
 *        @ref ECT_ASSERT_CONTAINS does, but with the addition of checking whether the item occurs as many times as
 *        specified. If it does not, it fails the test case and prints the message specified.
 * @param array The array to check.
 * @param item The item to look for.
 * @param cmpfunc The comparator function. Must be a function that takes two items of the type of @item and returns a
 *                integer. The function should return 1 if the items compare equal; else 0.
 * @param msg The message to print on failed assert.
 */
//TODO IMPLEMENT
#define ECT_ASSERT_CONTAINSN(array, item, noccurence, cmpfunc, msg)
#define ECT_FASSERT_CONTAINSN(array, item, noccurence, cmpfunc, fmt, ...)

//TODO add ECT_ASSERT_DCONTAINS, ECT_ASSERT_NOT_DCONTAINS and ECT_ASSERT_DCONTAINSN for dynamic arrays.
/**@}*/

/**
 * @defgroup UTILITIES Testing Utility Macros
 * @{
 */
/**
 * @def ECT_NO_SETUP
 * @brief Used in @ref ECT_DECLARE_BEFORE_TEST and @ref ECT_DECLARE_BEFORE_MODULE when there are no setup functions.
 */
#define ECT_NO_SETUP ECT_NO_SETUP__

/**
 * @def ECT_NO_TEARDOWN
 * @brief Used in @ref ECT_DECLARE_AFTER_TEST and @ref ECT_DECLARE_AFTER_MODULE when there are no teardown functions.
 */
#define ECT_NO_TEARDOWN ECT_NO_TEARDOWN__

/**
 * @def ECT_ARRAY_FOREACH(item, array)
 * @brief Utility macro for looping over each element in an array.
 * @param item The item yielded for each iteration.
 * @param array The array to loop over.
 * Loops over each element in an array. Note that it has to be an array, for pointers to arrays/dynamic arrays, see
 * @ref ECT_DARRAY_FOREACH.
 */
#define ECT_ARRAY_FOREACH(item, array) ECT_FOREACH__(item, array)

/**
 * @def ECT_DARRAY_FOREACH(item, array, size)
 * @brief Utility macro for looping over each element in an dynamic array or a pointer to a array.
 * @param item The item yielded for each iteration.
 * @param array The array to loop over.
 * @param nelem The number of elements in @p array.
 */
//TODO IMPLEMENT
#define ECT_DARRAY_FOREACH(item, array, nelem)

/**
 * @def ECT_LLIST_FOREACH(item, list, nextname)
 * @brief Utility macro to walk through each node in a non circular linked list.
 * @param node The list node yielded for each iteration.
 * @param list The list to walk through
 * @param nextname The name of the member that points ot the next node.
 * Walks through each node in a non circular linked list. In case of lists with a head-specific structure, the first
 * actual item-node must be supplied as @p list.
 * @p nextname should be the name of the member, in a list node, that points ot the next node. 
 */
//TODO IMPLEMENT
#define ECT_LLIST_FOREACH(node, list, nextname)

/**@}*/

/* END PUBLIC MACROS */









/// @cond INTERNAL
/* INTERNAL
 *
 * This section contains macros/functions/data structures for internal usage within the ECTest framework. Do NOT use
 * these macros when writing test modules and runners. See the public section for macros to use when writing test
 * modules and runners.
 *
 * Using these macros/functions/data structures for test modules and runners are considered undefined behaviour in
 * the context of using the ECTest framework.
 */

#ifndef ETC_LOG_LEVEL
#   define ETC_LOG_LEVEL 3
#endif

#ifdef __unix__
#   define ECT_USE_ANSI_ESCAPE__
#endif

#ifndef ECT_ABORT_ON_FAIL
#   define ECT_ABORT_ON_FAIL 0
#else
#   undef ECT_ABORT_ON_FAIL
#   define ECT_ABORT_ON_FAIL 1
#endif

#define ECT_FOREACH__(item, array) \
    for(int ECT_CONC__(array,keep__) = 1, \
            ECT_CONC__(array,count__) = 0,\
            ECT_CONC__(array,size__) = sizeof (array) / sizeof *(array); \
        ECT_CONC__(array,keep__) && ECT_CONC__(array,count__) != ECT_CONC__(array,size__); \
        ECT_CONC__(array,keep__) = !ECT_CONC__(array,keep__), ECT_CONC__(array,count__)++) \
      for(item = (array)[ECT_CONC__(array,count__)]; ECT_CONC__(array,keep__); ECT_CONC__(array,keep__) = !ECT_CONC__(array,keep__))

#define ECT_MODULE_RUNNER__(modulename) ECT_CONCWU__(modulename, run)

#define ECT_RUN_MODULE__(modulename)\
    ECT_IF__(ECT_ABORT_ON_FAIL,\
        do{\
            ECT_LOG_SEP__();\
            ECT_FLOG__("[Module Start] Running test module '%s'\n", ECT_STRINGIZE__(modulename));\
            ect_moduleresult result_ = ECT_MODULE_RUNNER__(modulename)();\
            if(result_.failed != 0){\
                ECT_LOG_SEP__();\
                ECT_LOG__("[Test Run Summary] | Module "#modulename" failed, aborting test run...\n");\
                return -1;/*TODO setup test evaluation*/\
            }\
        }while(0),\
        ECT_MODULE_RUNNER__(modulename)();\
    )

/*** UTILITY MACROS ***/
#define ECT_PCON__(x, y) x##y
#define ECT_CONC__(x, y) ECT_PCON__(x, y)
#define ECT_PCONCWU__(x, y) x##_##y
#define ECT_CONCWU__(x, y) ECT_PCONCWU__(x,y)
#define ECT_ESC_PAR__(...) __VA_ARGS__
#define ECT_IF___1(t, f) t
#define ECT_IF___0(t, f) f
#define ECT_IF__(c, t, f) ECT_CONCWU__(ECT_IF__, c)(ECT_ESC_PAR__(t),ECT_ESC_PAR__(f))
#define ECT_PSTRINGIZE__(x) #x
#define ECT_STRINGIZE__(x) ECT_PSTRINGIZE__(x)

/*** LOOP BASE MACRO ***/
#define ECT_ARG_N__(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define ECT_RSEQ_N__() 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define ECT_NARG___(...) ECT_ARG_N__(__VA_ARGS__)
#define ECT_NARG__(...) ECT_NARG___(__VA_ARGS__, ECT_RSEQ_N__())

#define ECT_LOOP___(loop_pattern, N, ...) ECT_CONC__(loop_pattern, N)(__VA_ARGS__)

#define ECT_LOOP__(loop_pattern, ...) ECT_LOOP___(loop_pattern, ECT_NARG__(__VA_ARGS__), __VA_ARGS__)

/*** END LOOP BASE MACRO ***/
/*** END UTILITY MACROS ***/

/*** FUNCTION POINTER DEFINES ***/
#define ECT_SIZEOFARRAY__(arrname) \
    (sizeof(arrname)/sizeof(*(arrname)))

#define ECT_FUNC_PTR__(type, name) type(*name)(void)
#define ECT_FUNC_PTR_ARRAY__(type, arrayname) ECT_FUNC_PTR__(type, arrayname[])
#define ECT_FUNC_PTR_ARRAYSIZE__(arrayname) arrayname##_size
#define ECT_DECL_FUNC_PTR_ARRAYSIZE__(arrayname) size_t ECT_FUNC_PTR_ARRAYSIZE__(arrayname)
/*** END FUNCTION POINTER DEFINES ***/

/*** TEST/SETUP/TEARDOWN DECLARES AND DEFINES ***/
#define ECT_DECLFUNC__(type, name) static type name(void)

#define ECT_DECLARE_TESTS__(...) ECT_EXPORT_FUNCS__(TARRAY, ect_caseresult, __VA_ARGS__)
#define ECT_DECLARE_BEFORE_MODULE__(...) ECT_EXPORT_FUNCS__(BMARRAY, void, __VA_ARGS__)
#define ECT_DECLARE_AFTER_MODULE__(...) ECT_EXPORT_FUNCS__(AMARRAY, void, __VA_ARGS__)
#define ECT_DECLARE_BEFORE_TEST__(...) ECT_EXPORT_FUNCS__(BTARRAY, void, __VA_ARGS__)
#define ECT_DECLARE_AFTER_TEST__(...) ECT_EXPORT_FUNCS__(ATARRAY, void, __VA_ARGS__)

#define ECT_TEST__(funcname) ECT_DECLFUNC__(ect_caseresult, funcname)
#define ECT_BEFORE_TEST__(funcname) ECT_DECLFUNC__(void, funcname)
#define ECT_AFTER_TEST__(funcname) ECT_DECLFUNC__(void, funcname)
#define ECT_BEFORE_MODULE__(funcname) ECT_DECLFUNC__(void, funcname)
#define ECT_AFTER_MODULE__(funcname) ECT_DECLFUNC__(void, funcname)

/*** END TEST/SETUP/TEARDOWN DECLARES AND DEFINES ***/

/*** MODULE IMPORT ***/
#define ECT_IMPORT_MODULE__(modulename)\
    extern ect_moduleresult ECT_CONCWU__(modulename, run)(void)

#define ECT_IMPORT_MODULES__(...)\
    ECT_LOOP__(ECT_IMPORT_MODULE__, __VA_ARGS__)
/*** END MODULE IMPORT ***/

/*** MODULE EXPORT ***/
#define ECT_EXPORT_FUNCS__(arrname, type, ...)\
    ECT_DECLEXPORT_FUNC__(type, __VA_ARGS__)\
    static ECT_FUNC_PTR_ARRAY__(type, arrname) = {\
    ECT_LOOP__(ECT_EXPORT_FUNC__, __VA_ARGS__)\
    };\
    static ECT_DECL_FUNC_PTR_ARRAYSIZE__(arrname) = ECT_SIZEOFARRAY__(arrname)

#define ECT_EXPORT_MODULE__(modulename) \
    ect_moduleresult ECT_CONCWU__(modulename, run)(void)\
    {\
        struct ect_moduleresult result_ = {0};\
            /*Run module setup functions*/\
            ECT_FOREACH__(ECT_FUNC_PTR__(void, bmfunc), BMARRAY){ bmfunc(); }\
            /*Loop over all test functions*/\
            ECT_FOREACH__(ECT_FUNC_PTR__(ect_caseresult, tfunc), TARRAY){\
                /*Run test setup functions*/\
                ECT_FOREACH__(ECT_FUNC_PTR__(void, btfunc), BTARRAY){ btfunc(); }\
                /*Run test function*/\
                result_.count++;\
                switch(tfunc()){\
                    case ECT_CASERES_SUCCESS:{\
                        result_.success++;\
                    }break;\
                    case ECT_CASERES_FAILED:{\
                        if(ECT_ABORT_ON_FAIL){\
                            ECT_LOG_SEP__();\
                            ECT_FLOG__("[Module Summary] Module '%s' failed, aborting test...\n\n",ECT_STRINGIZE__(modulename));\
                            return result_;\
                        }\
                        result_.failed++;\
                    }break;\
                    case ECT_CASERES_SKIPPED:{\
                        result_.skipped++;\
                    }break;\
                }\
                /*Run test teardown functions*/\
                ECT_FOREACH__(ECT_FUNC_PTR__(void, atfunc), BTARRAY){ atfunc(); }\
        }\
        /*Run module teardown functions*/\
        ECT_FOREACH__(ECT_FUNC_PTR__(void, amfunc), AMARRAY){ amfunc(); }\
        ECT_FLOG__("[Module Summary] Module '%s' | Tests: %d Success: %d Failed: %d Skipped: %d\n", ECT_STRINGIZE__(modulename),\
                result_.count, result_.success, result_.failed, result_.skipped);\
        ECT_LOG_SEP__();\
        return result_;\
    }

/*** END MODULE EXPORT ***/

/*** EXPORT LOOPS ***/
#define ECT_EXPORT_FUNC__1(arg) &arg
#define ECT_EXPORT_FUNC__2(arg, ...) &arg, ECT_EXPORT_FUNC__1(__VA_ARGS__)
#define ECT_EXPORT_FUNC__3(arg, ...) &arg, ECT_EXPORT_FUNC__2(__VA_ARGS__)
#define ECT_EXPORT_FUNC__4(arg, ...) &arg, ECT_EXPORT_FUNC__3(__VA_ARGS__)
#define ECT_EXPORT_FUNC__5(arg, ...) &arg, ECT_EXPORT_FUNC__4(__VA_ARGS__)
#define ECT_EXPORT_FUNC__6(arg, ...) &arg, ECT_EXPORT_FUNC__5(__VA_ARGS__)
#define ECT_EXPORT_FUNC__7(arg, ...) &arg, ECT_EXPORT_FUNC__6(__VA_ARGS__)
#define ECT_EXPORT_FUNC__8(arg, ...) &arg, ECT_EXPORT_FUNC__7(__VA_ARGS__)
#define ECT_EXPORT_FUNC__9(arg, ...) &arg, ECT_EXPORT_FUNC__8(__VA_ARGS__)
#define ECT_EXPORT_FUNC__10(arg, ...) &arg, ECT_EXPORT_FUNC__9(__VA_ARGS__)
#define ECT_EXPORT_FUNC__11(arg, ...) &arg, ECT_EXPORT_FUNC__10(__VA_ARGS__)
#define ECT_EXPORT_FUNC__12(arg, ...) &arg, ECT_EXPORT_FUNC__11(__VA_ARGS__)
#define ECT_EXPORT_FUNC__13(arg, ...) &arg, ECT_EXPORT_FUNC__12(__VA_ARGS__)
#define ECT_EXPORT_FUNC__14(arg, ...) &arg, ECT_EXPORT_FUNC__13(__VA_ARGS__)
#define ECT_EXPORT_FUNC__15(arg, ...) &arg, ECT_EXPORT_FUNC__14(__VA_ARGS__)
#define ECT_EXPORT_FUNC__16(arg, ...) &arg, ECT_EXPORT_FUNC__15(__VA_ARGS__)
#define ECT_EXPORT_FUNC__17(arg, ...) &arg, ECT_EXPORT_FUNC__16(__VA_ARGS__)
#define ECT_EXPORT_FUNC__18(arg, ...) &arg, ECT_EXPORT_FUNC__17(__VA_ARGS__)
#define ECT_EXPORT_FUNC__19(arg, ...) &arg, ECT_EXPORT_FUNC__18(__VA_ARGS__)

#define ECT_DECLEXPORT_FUNC__1(type, arg) ECT_DECLFUNC__(type, arg);
#define ECT_DECLEXPORT_FUNC__2(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__1(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__3(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__2(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__4(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__3(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__5(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__4(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__6(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__5(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__7(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__6(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__8(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__7(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__9(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__8(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__10(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__9(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__11(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__10(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__12(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__11(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__13(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__12(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__14(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__13(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__15(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__14(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__16(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__15(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__17(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__16(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__18(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__17(type, __VA_ARGS__)
#define ECT_DECLEXPORT_FUNC__19(type, arg, ...) ECT_DECLFUNC__(type, arg); ECT_DECLEXPORT_FUNC__18(type, __VA_ARGS__)

#define ECT_DECLEXPORT_FUNC__(type, ...) \
    ECT_LOOP___(ECT_DECLEXPORT_FUNC__, ECT_NARG__(__VA_ARGS__), type, __VA_ARGS__)
/*** END EXPORT LOOPS ***/

/*** IMPORT LOOPS ***/
#define ECT_IMPORT_MODULE__1(arg) ECT_IMPORT_MODULE(arg)
#define ECT_IMPORT_MODULE__2(arg, ...) ECT_IMPORT_MODULE(arg); ECT_IMPORT_MODULE__1(__VA_ARGS__)
#define ECT_IMPORT_MODULE__3(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__2(__VA_ARGS__)
#define ECT_IMPORT_MODULE__4(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__3(__VA_ARGS__)
#define ECT_IMPORT_MODULE__5(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__4(__VA_ARGS__)
#define ECT_IMPORT_MODULE__6(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__5(__VA_ARGS__)
#define ECT_IMPORT_MODULE__7(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__6(__VA_ARGS__)
#define ECT_IMPORT_MODULE__8(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__7(__VA_ARGS__)
#define ECT_IMPORT_MODULE__9(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__8(__VA_ARGS__)
#define ECT_IMPORT_MODULE__10(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__9(__VA_ARGS__)
#define ECT_IMPORT_MODULE__11(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__10(__VA_ARGS__)
#define ECT_IMPORT_MODULE__12(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__11(__VA_ARGS__)
#define ECT_IMPORT_MODULE__13(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__12(__VA_ARGS__)
#define ECT_IMPORT_MODULE__14(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__13(__VA_ARGS__)
#define ECT_IMPORT_MODULE__15(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__14(__VA_ARGS__)
#define ECT_IMPORT_MODULE__16(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__15(__VA_ARGS__)
#define ECT_IMPORT_MODULE__17(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__16(__VA_ARGS__)
#define ECT_IMPORT_MODULE__18(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__17(__VA_ARGS__)
#define ECT_IMPORT_MODULE__19(arg, ...) ECT_IMPORT_MODULE(arg) ECT_IMPORT_MODULE__18(__VA_ARGS__)
/*** END IMPORT LOOPS ***/

/*** END MODULE EXPORT ***/

/*** LOGGING ***/

#ifdef ECT_USE_ANSI_ESCAPE__
#   define ECT_PRINT_WITH_COLOR__(color, txt) color txt ECT_KNRM__
#   define ECT_KNRM__  "\x1B[0m"
#   define ECT_KRED__  "\x1B[31m"
#   define ECT_KGRN__  "\x1B[32m"
#   define ECT_KYEL__  "\x1B[33m"
#   define ECT_KBLU__  "\x1B[34m"
#   define ECT_KMAG__  "\x1B[35m"
#   define ECT_KCYN__  "\x1B[36m"
#   define ECT_KWHT__  "\x1B[37m"
#else
#   define ECT_PRINT_WITH_COLOR__(color, txt) txt
#endif

#define ECT_FLOG__(fmt, ...) printf(fmt, __VA_ARGS__)
#define ECT_LOG__(msg) ECT_FLOG__("%s", msg)
#define ECT_LOG_SEP__()\
    ECT_LOG__("------------------------------------------------------------------\n")
/*** END LOGGING ***/

/*** TEST CASE EXITS ***/
#if ETC_LOG_LEVEL >= 1
#   define ECT_FFAIL__(fmt, ...) \
        do{\
            ECT_FLOG__("["ECT_PRINT_WITH_COLOR__(ECT_KRED__, "FAILED")"]\t%s | "fmt"\n", __func__, __VA_ARGS__);\
            return ECT_CASERES_FAILED;\
        }while(0)
#else
#   define ECT_FFAIL__(fmt, ...) return ECT_CASERES_FAILED
#endif

#define ECT_FAIL__(msg) ECT_FFAIL("%s", msg)

#if ETC_LOG_LEVEL >= 2
#   define ECT_SKIP__() \
        do{\
            ECT_FLOG__("["ECT_PRINT_WITH_COLOR__(ECT_KYEL__,"SKIPPED")"]\t%s\n", __func__);\
            return ECT_CASERES_SKIPPED;\
        }while(0)
#else
#   define ECT_SKIP__() return ECT_CASERES_SKIPPED
#endif

#if ETC_LOG_LEVEL >= 3
#   define ECT_SUCCESS__() \
        do{\
            ECT_FLOG__("["ECT_PRINT_WITH_COLOR__(ECT_KGRN__,"SUCCESS")"]\t%s\n", __func__);\
            return ECT_CASERES_SUCCESS;\
        }while(0)
#else
#   define ECT_SUCCESS__() return ECT_CASERES_SUCCESS
#endif
/*** END TEST CASE EXITS ***/

/*** ASSERT ***/
#define ECT_FASSERT__(cond, failfmt, ...)            \
    do{                                         \
        if(!(cond)){                            \
            ECT_FFAIL(failfmt, __VA_ARGS__);        \
        }                                       \
    }while(0)

#define ECT_ASSERT__(cond, failmsg) ECT_FASSERT__(cond, "%s", failmsg)
/*** END ASSERT ***/

/*** RESULT STRUCTS AND DUMMY FUNCTIONS ***/
typedef struct ect_moduleresult{
    int count;
    int success;
    int failed;
    int skipped;
}ect_moduleresult;

typedef enum ect_caseresult{
    ECT_CASERES_SUCCESS,
    ECT_CASERES_FAILED,
    ECT_CASERES_SKIPPED
}ect_caseresult;

static inline void ect_nosetupteardown__(void){}
#define ECT_NO_SETUP__ ect_nosetupteardown__
#define ECT_NO_TEARDOWN__ ect_nosetupteardown__
/*** END RESULT STRUCTS AND DUMMY FUNCTIONS ***/
/// @endcond
#endif // _ECTEST_H__