#include "CXmlParser.h"
#include "../cxmltree/CXmlNode.h"
#include "../cxmltree/CxmlDocument.h"
#include "../utils/cxml_stack.h"
#include "../utils/cxml_queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>


enum CXmlParserTagType{
    CXML_PTT_OPENING,
    CXML_PTT_CLOSING,
    CXML_PTT_OPENCLOSE,
};

enum CXmlParserState{
    CXML_PS_INNER,
    CXML_PS_RDY
};

struct CXmlParserFrame{
    char *tag;
    size_t depth;
    CXmlNodeType type;
    struct CXmlParserFrame *parent;
    struct CXmlParserFrameList *children;
};

struct CXmlParserFrameList{
    struct CXmlParserFrame *frame;
    struct CXmlParserFrameList *next;
    struct CXmlParserFrameList *prev;
};

struct CXmlParser{
    struct CXmlParserFrame *curframe;
    struct CXmlParserFrame *rootframe;
    enum CXmlParserState state;
    unsigned depth;
    int error;
    char *buf;
    size_t bufsize;
};

#define CXML_PARSER_WRITE_TO_STORE(parser, buf, offset)\
    do{                                                         \
        size_t nsize = (parser)->bufsize + (offset);            \
        if(nsize == 0) break;                                   \
        (parser)->buf = realloc((parser)->buf, nsize);          \
        strncpy((parser)->buf + (parser)->bufsize, buf, offset);\
        (parser)->bufsize = nsize;                              \
        (offset) = 0;                                           \
    }while(0)

static void cxml_parsestring(CXmlParser *parser, const char *line);
static char *cxml_readtagname(size_t len, char **tag, const char *line, size_t *loopinc, enum CXmlParserTagType *tagtype);

static long long cxml_parser_getline(char **lineptr, size_t *n, FILE *stream);
static char *cxml_parser_trimwhitespace(const char *str);

static struct CXmlParserFrame *cxml_frame_new(const char *tag, size_t depth, struct CXmlParserFrame *parent, CXmlNodeType type);
static void cxml_frame_appendchild(struct CXmlParserFrame *frame, struct CXmlParserFrame *child);
static void cxml_parser_finalizeNodeTree(struct CXmlParserFrame *frame,
                                         CXmlDocument *doc);
static void cxml_parser_freeframetree(struct CXmlParserFrame *frame);

void cxml_parser_traverseframetree(struct CXmlParserFrame *frame); //TODO REMOVE

CXmlParser *CXmlParser_new()
{
    CXML_DEBUG("Creating parser");
    CXmlParser *parser = malloc(sizeof *parser);
    parser->depth = 0;
    parser->error = CXML_ESUCCESS;
    parser->state = CXML_PS_RDY;
    parser->curframe = NULL;
    parser->rootframe = NULL;
    parser->bufsize = 0;
    parser->buf = NULL;
    return parser;
}

CXmlDocument *CXmlParser_parsefile(CXmlParser *parser, const char *filename)
{
    CXML_DEBUGF("Opening file: %s", filename);
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        CXML_DEBUGF("No file found: %s", filename);
        parser->error = CXML_ERRNO;
        return NULL;
    }
    char *line = NULL;
    size_t n;
    long long nread;

    CXML_DEBUG("Begin reading");
    while((nread = cxml_parser_getline(&line, &n, fp)) != -1){
        //CXML_DEBUGF("Read line: %s", line);
        cxml_parsestring(parser, line);
    }
    CXML_DEBUG("Reading done");
    free(line);
    fclose(fp);
    CXML_DEBUGF("Closed file: %s", filename);

    CXML_DEBUG("Printing frame tree");
    cxml_parser_traverseframetree(parser->rootframe);

    CXmlDocument *doc = CXmlDocument_new();
    cxml_parser_finalizeNodeTree(parser->rootframe, doc);
    cxml_parser_freeframetree(parser->rootframe);

    free(parser->buf);
    parser->bufsize = 0;
    return doc;
}

CXmlDocument *cxml_parsexml(CXmlParser *parser, const char **xml, size_t lines)
{
    //TODO implement
    return NULL;
}

static void cxml_parsestring(CXmlParser *parser, const char *line)
{
    static int enterCount = 0;
    char *trimmedline = cxml_parser_trimwhitespace(line);
    size_t linelen = strlen(trimmedline);
    enum CXmlParserTagType tagtype;
    char *tag = NULL;
    char buf[256] = {0};
    unsigned offset = 0;
    for (size_t i = 0; i < linelen ; ++i) {

        if(trimmedline[i] == '<'){
            cxml_readtagname(linelen, &tag, trimmedline, &i, &tagtype);
            if(tagtype == CXML_PTT_CLOSING){
                CXML_DEBUGF("Offset: %d", offset);
                if(offset > 0 || parser->bufsize > 0){
                    char txtTag[offset+parser->bufsize+1];
                    if(parser->bufsize > 0) strncpy(txtTag, parser->buf, parser->bufsize);
                    strncpy(txtTag+parser->bufsize, buf, offset);
                    txtTag[parser->bufsize] = '\0';
                    struct CXmlParserFrame *txtFrame = cxml_frame_new(txtTag, parser->depth, parser->curframe, CXML_TEXT);
                    cxml_frame_appendchild(parser->curframe, txtFrame);
                    free(parser->buf);
                    parser->buf = NULL;
                    parser->bufsize = 0;
                }
                /*CXML_PARSER_WRITE_TO_STORE(parser, buf, offset);
                if(parser->bufsize > 0){
                    CXML_DEBUG("We made it");

                    char txtTag[parser->bufsize+1];
                    strncpy(txtTag, parser->buf, parser->bufsize);
                    txtTag[parser->bufsize] = '\0';
                    struct CXmlParserFrame *txtFrame = cxml_frame_new(txtTag, parser->depth, parser->curframe, CXML_TEXT);
                    cxml_frame_appendchild(parser->curframe, txtFrame);
                    free(parser->buf);
                    parser->buf = NULL;
                    parser->bufsize = 0;

                    //TODO moving curframe->text to CXML_TEXT node
                    //parser->curframe->text = malloc(parser->bufsize+1);
                    //strncpy(parser->curframe->text, parser->buf, parser->bufsize);
                    //parser->curframe->text[parser->bufsize] = '\0';
                    //free(parser->buf);
                    //parser->buf = NULL;
                    //parser->bufsize = 0;
                }*/

                parser->depth--;
                parser->curframe = parser->curframe->parent;
            }

            if(tagtype == CXML_PTT_OPENING || tagtype == CXML_PTT_OPENCLOSE){
                struct CXmlParserFrame *elemFrame = cxml_frame_new(tag, parser->depth, parser->curframe, CXML_ELEMENT);

                if(parser->depth == 0) parser->rootframe = elemFrame;
                if(parser->curframe != NULL) cxml_frame_appendchild(parser->curframe, elemFrame);

                if(tagtype == CXML_PTT_OPENING){
                    parser->curframe = elemFrame;
                    parser->depth++;
                    parser->state = CXML_PS_INNER;
                }

            }
            continue;
        }
        if(offset+1 == sizeof buf){
            CXML_PARSER_WRITE_TO_STORE(parser, buf, offset);
        }
        buf[offset] = trimmedline[i];
        offset++;
    }

    if(offset > 0 || parser->bufsize > 0){
        char txtTag[offset+parser->bufsize+1];
        strncpy(txtTag, parser->buf, parser->bufsize);
        strncpy(txtTag+parser->bufsize, buf, offset);
        txtTag[offset+parser->bufsize] = '\0';
        struct CXmlParserFrame *txtFrame = cxml_frame_new(txtTag, parser->depth, parser->curframe, CXML_TEXT);
        cxml_frame_appendchild(parser->curframe, txtFrame);
        free(parser->buf);
        parser->buf = NULL;
        parser->bufsize = 0;
    }
    /*if(offset > 0){
        CXML_PARSER_WRITE_TO_STORE(parser, buf, offset);
    }*/
    free(tag);
    free(trimmedline);
}

/*Parser support functions*/
static char *cxml_readtagname(size_t len, char **tag, const char *line, size_t *loopinc, enum CXmlParserTagType *tagtype)
{
    size_t origin = *loopinc;
    CXML_DEBUGF("Origin: %zu", origin);
    size_t index;
    char buf[len];
    for (index = 0; index+origin < len; index++) {
        //TODO check for naughty formatters
        if(line[index+origin] == '>'){
            buf[index] = line[index+origin];
            break;
        }
        buf[index] = line[index+origin];
    }

    size_t rsize = 0;
    buf[index+1] = '\0';
    int closersFound = 0;

    if(buf[1] == '/'){
        closersFound++;
        buf[index] = '\0';
        rsize = strlen(buf)-1;
        *tagtype = CXML_PTT_CLOSING;
    }
    if(buf[index-1] == '/'){
        closersFound++;
        rsize = strlen(buf)-2;
        buf[index-1] = '\0';
        *tagtype = CXML_PTT_OPENCLOSE;
    }
    if(!closersFound){
        buf[index] = '\0';
        rsize = strlen(buf);
        *tagtype = CXML_PTT_OPENING;
    }

    if(closersFound > 1){
        //TODO error handling
    }

    *tag = realloc(*tag, rsize);
    strcpy(*tag, buf + (buf[1] == '/' ? 2 : 1));
    CXML_DEBUGF("Tag name: %s", *tag);
    *loopinc = index+origin;
    CXML_DEBUGF("new index:%zu", *loopinc);
    return NULL;
}

/*I/O and sanitation*/
static long long cxml_parser_getline(char **lineptr, size_t *n, FILE *stream)
{
    char c, *cur_pos, *new_lineptr;
    size_t new_lineptr_len;

    if (lineptr == NULL || n == NULL || stream == NULL) {
        return -1;
    }

    if (*lineptr == NULL) {
        *n = 128; /* init len */
        if ((*lineptr = (char *)malloc(*n)) == NULL) {
            return -1;
        }
    }

    cur_pos = *lineptr;
    for (;;) {
        c = (char)getc(stream);

        if (ferror(stream) || (c == EOF && cur_pos == *lineptr))
            return -1;

        if (c == EOF)
            break;

        if ((*lineptr + *n - cur_pos) < 2) {
            if (LLONG_MAX / 2 < *n) {
                return -1;
            }
            new_lineptr_len = *n * 2;

            if ((new_lineptr = (char *)realloc(*lineptr, new_lineptr_len)) == NULL) {
                return -1;
            }
            cur_pos = new_lineptr + (cur_pos - *lineptr);
            *lineptr = new_lineptr;
            *n = new_lineptr_len;
        }

        *cur_pos++ = c;

        if (c == '\n')
            break;
    }

    *cur_pos = '\0';
    return (long long)(cur_pos - *lineptr);
}

static char *cxml_parser_trimwhitespace(const char *str)
{
    char *buf = malloc(strlen(str)+1);
    char *origbuf = buf;
    strcpy(buf, str);
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*buf)) buf++;

    if(*buf == 0) return buf;

    // Trim trailing space
    end = buf + strlen(buf) - 1;
    while(end > buf && isspace((unsigned char)*end)) end--;

    end[1] = '\0';
    char *ret = malloc(strlen(buf)+1);
    strcpy(ret, buf);
    free(origbuf);
    return ret;
}

/*Parser frame functions*/
static struct CXmlParserFrame *cxml_frame_new(const char *tag, size_t depth, struct CXmlParserFrame *parent, CXmlNodeType type)
{
    struct CXmlParserFrame *frame = malloc(sizeof *frame);
    if(tag != NULL) {
        frame->tag = malloc(strlen(tag) + 1);
        strcpy(frame->tag, tag);
    }else{
        frame->tag = NULL;
    }
    frame->children = malloc(sizeof *frame->children);
    frame->children->next = NULL;
    frame->children->prev = NULL;
    frame->children->frame = NULL;
    frame->parent = parent;
    frame->depth = depth;
    frame->type = type;
    return frame;
}

static void cxml_frame_appendchild(struct CXmlParserFrame *frame, struct CXmlParserFrame *child)
{
    struct CXmlParserFrameList *list = frame->children;
    struct CXmlParserFrameList *new = malloc(sizeof *new);
    new->frame = child;

    if(list->next == NULL){
        list->next = new;
        list->prev = new;
        new->next = NULL;
        new->prev = NULL;
        return;
    }

    new->prev = list->prev;
    list->prev->next = new;
    new->next = NULL;
    list->prev = new;
}

//TODO remove
void cxml_parser_traverseframetree(struct CXmlParserFrame *frame)
{
    if(frame == NULL) return;

    CXmlQueue *queue = cxml_queue_new();

    struct CXmlParserFrame *curframe;
    struct CXmlParserFrameList *curchild;
    cxml_queue_enqueue(queue, frame);
    while(!cxml_queue_isEmpty(queue)){
        curframe = cxml_queue_dequeue(queue);
        curchild = curframe->children->next;

        while(curchild != NULL){
            cxml_queue_enqueue(queue, curchild->frame);
            curchild = curchild->next;
        }

        CXML_DEBUGF("Tag: %s", curframe->tag);
        CXML_DEBUGF("Depth: %zu",curframe->depth);
        CXML_DEBUGF("Parent: %s", curframe->parent != NULL ? curframe->parent->tag : "none");
    }

    cxml_queue_destroy(queue);
}

static void cxml_parser_freeframetree(struct CXmlParserFrame *frame)
{
    if(frame == NULL) return;
    cxml_stack *stack = cxml_stack_new();

    struct CXmlParserFrame *curframe;
    struct CXmlParserFrameList *curchild;
    struct CXmlParserFrameList *tmpfree;
    cxml_stack_push(stack, frame);
    while(!cxml_stack_isempty(stack)){
        curframe = cxml_stack_pop(stack);
        curchild = curframe->children->next;

        while(curchild != NULL){
            cxml_stack_push(stack, curchild->frame);
            tmpfree = curchild;
            curchild = curchild->next;
            free(tmpfree);
        }

        free(curframe->tag);
        free(curframe->children);
        free(curframe);
    }
    cxml_stack_destroy(stack);
}

static void cxml_parser_finalizeNodeTree(struct CXmlParserFrame *frame,
                                         CXmlDocument *doc)
{
    if(frame == NULL || doc == NULL) return;

    CXmlQueue *queue = cxml_queue_new();

    struct CXmlParserFrame *curframe;
    struct CXmlParserFrameList *curchild;
    struct CXmlNode *curparent;
    struct CXmlNode *curnode;

    cxml_queue_enqueue(queue, doc->docnode);
    cxml_queue_enqueue(queue, frame);

    while(!cxml_queue_isEmpty(queue)){
        curparent = cxml_queue_dequeue(queue);
        curframe = cxml_queue_dequeue(queue);
        curchild = curframe->children->next;

        curnode = CXmlNode_new(curframe->type, curframe->tag,
                               NULL /*TODO remove text from node*/, curparent);
        if(curparent != NULL){
            if(curparent->type == CXML_DOCUMENT){
                CXmlDocument_setRoot(doc, curnode);
            }else {
                CXmlNode_appendChild(curparent, curnode);
            }
        }

        while(curchild != NULL){
            cxml_queue_enqueue(queue, curnode);
            cxml_queue_enqueue(queue, curchild->frame);
            curchild = curchild->next;
        }
    }
    cxml_stack_destroy(queue);
}

