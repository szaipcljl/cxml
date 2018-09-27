#ifndef _CXML_PARSER_H__
#define _CXML_PARSER_H__

#include "../cxmltree/cxml_document.h"

#ifndef CXML_PARSER_BUFSIZE
#define CXML_PARSER_BUFSIZE 1024
#endif

typedef struct cxml_parser cxml_parser; //opaque structure, use accessor funcs.
cxml_parser *cxml_parser_new();
cxml_document *cxml_parsefile(cxml_parser *parser, const char *filename);
#define cxml_parser_destroy(ptr) free(ptr)
#endif // _CXML_PARSER_H__