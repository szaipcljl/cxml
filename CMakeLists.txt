cmake_minimum_required(VERSION 3.10)
project(cxml C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pedantic -Wall -Wextra -Werror -g -Wno-unused")

add_library(cxml
        src/cxmltree/cxml_node.c
        src/cxmltree/cxml_node.h
        src/cxmltree/cxml_document.c
        src/cxmltree/cxml_document.h
        src/parser/cxml_parser.c
        src/parser/cxml_parser.h src/utils/cxml_stack.c src/utils/cxml_stack.h)

add_executable(cxmltest test/cxmltest.c)

target_link_libraries(cxmltest cxml)
#link_libraries(cxmltest cxml)