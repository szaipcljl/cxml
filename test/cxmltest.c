#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TEST_ABORT_ON_FAIL

#include "CXmlTestSetup.h"

TEST_IMPORT_MODULE(CXmlStringTest);

#define TEST_IMPORT_MODULES(...)\
    TEST_LOOP__(TEST_IMPORT_MODULE__, __VA_ARGS__)

TEST_IMPORT_MODULES(CXmlStringTest, TestModule);

#define TEST_RUN_MODULE(modulename)\
    TEST_IF__(TEST_ABORT_ON_FAIL,\
        do{\
            if(modulename##_run() != 0){\
                TEST_LOG_SEP__();\
                TEST_LOG__("[Test Run Summary] | Module "#modulename" failed, aborting test run...\n");\
                return -1;/*TODO setup test evaluation*/\
            }\
        }while(0),\
        modulename##_run()\
    )

int main(){
    //TEST_RUN_MODULE(CXmlStringTest);
    TEST_RUN_MODULE(TestModule);
    return 0;
}