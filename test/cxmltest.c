#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/parser/CXmlParser.h"

int main(){
    CXML_DEBUG("Starting");
    CXmlParser *parser = CXmlParser_new();
    char *filepath = "../test.xml";

    //strcat(strcpy(filepath, getenv("HOME")), "/private/gitrepos/cxml/test.xml");
    CXmlDocument *doc = CXmlParser_parsefile(parser, filepath);

    unsigned i;
    /*CXmlNode **nodes = CXmlNode_getChildrenByTag(doc->root, "first", &i);
    if(nodes != NULL) CXML_DEBUG("OOOOH BABY!");
    free(nodes);*/

    CXmlDocument_destroy(doc);

    CXML_DEBUG("Returned from CXmlParser_parsefile");
    CxmlParser_destroy(parser);
    CXML_DEBUG("Exiting");
    return 0;
}