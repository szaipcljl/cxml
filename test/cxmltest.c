#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define ECT_ABORT_ON_FAIL

#include "ectest.h"

ECT_IMPORT_MODULE(CXmlStringTest);

ECT_IMPORT_MODULES(CXmlStringTest, TestModule);

int main(){
    //ECT_RUN_MODULE(CXmlStringTest);
    ECT_RUN_MODULE(TestModule);
    return 0;
    ect_caseresult rs;

}