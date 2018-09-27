#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/parser/cxml_parser.h"

int main(){
    CXML_DEBUG("Starting");
    cxml_parser *parser = cxml_parser_new();
    char filepath[256];

    strcat(strcpy(filepath, getenv("HOME")), "/private/gitrepos/cxml/test.xml");
    cxml_document *doc = cxml_parsefile(parser, filepath);

    unsigned i;
    cxml_node **nodes = cxml_node_getChildrenByTag(doc->root, "first", &i);
    if(nodes != NULL) CXML_DEBUG("OOOOH BABY!");
    free(nodes);

    cxml_document_destroy(doc);

    CXML_DEBUG("Returned from cxml_parsefile");
    cxml_parser_destroy(parser);
    CXML_DEBUG("Exiting");
    return 0;
}