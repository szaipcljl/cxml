cmake_minimum_required(VERSION 3.10)
project(cxml C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pedantic -Wall -Wextra -Werror -g -Wno-unused -DNOT_IMPLEMENTED='exit(EXIT_FAILURE);'")

add_library(cxml
        src/cxmltree/CXmlNode.c
        src/cxmltree/CXmlNode.h
        src/cxmltree/CxmlDocument.c
        src/cxmltree/CxmlDocument.h
        src/parser/CXmlParser.c
        src/parser/CXmlParser.h src/utils/cxml_stack.c src/utils/cxml_stack.h src/utils/CXmlQueue.c src/utils/CXmlQueue.h src/utils/cxml_preproc.h src/utils/CXmlString.c src/utils/CXmlString.h)

add_executable(cxmltest test/cxmltest.c)

target_link_libraries(cxmltest cxml)
#link_libraries(cxmltest cxml)