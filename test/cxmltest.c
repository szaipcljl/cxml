#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/parser/cxml_parser.h"

int main(){
    CXML_DEBUG("Starting");
    cxml_parser *parser = cxml_parser_new();
    char filepath[256];

    strcat(strcpy(filepath, getenv("HOME")), "/private/gitrepos/cxml/test.xml");
    cxml_parsefile(parser, filepath);

    CXML_DEBUG("Returned from cxml_parsefile");
    cxml_parser_destroy(parser);
    CXML_DEBUG("Exiting");
    return 0;
}