#ifndef _CXML_PARSER_H__
#define _CXML_PARSER_H__

#include "../cxmltree/CxmlDocument.h"

#ifndef CXML_PARSER_BUFSIZE
#define CXML_PARSER_BUFSIZE 1024
#endif

typedef struct CXmlParser CXmlParser; //opaque structure, use accessor funcs.
CXmlParser *CXmlParser_new();
CXmlDocument *CXmlParser_parsefile(CXmlParser *parser, const char *filename);
#define CxmlParser_destroy(ptr) free(ptr)
#endif // _CXML_PARSER_H__