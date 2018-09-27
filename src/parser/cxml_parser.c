#include "cxml_parser.h"
#include "../cxmltree/cxml_node.h"
#include "../cxmltree/cxml_document.h"
#include "../utils/cxml_stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>


enum cxml_ptagtype{
    CXML_PTT_OPENING,
    CXML_PTT_CLOSING,
    CXML_PTT_OPENCLOSE,
};

enum cxml_pstate{
    CXML_PS_INNER,
    CXML_PS_RDY
};

struct cxml_parser_frame{
    char *tag;
    char *innerText;
    size_t depth;
    struct cxml_parser_frame *parent;
    struct cxml_parser_framelist *children;
};

struct cxml_parser_framelist{
    struct cxml_parser_frame *frame;
    struct cxml_parser_framelist *next;
    struct cxml_parser_framelist *prev;
};

struct cxml_parser{
    struct cxml_parser_frame *curframe;
    struct cxml_parser_frame *rootframe;
    enum cxml_pstate state;
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

static void cxml_parsestring(cxml_parser *parser, const char *line);
static char *cxml_readtagname(size_t len, char **tag, const char *line, size_t *loopinc, enum cxml_ptagtype *tagtype);

static long long cxml_parser_getline(char **lineptr, size_t *n, FILE *stream);
static char *cxml_parser_trimwhitespace(const char *str);

static struct cxml_parser_frame *cxml_frame_new(const char *tag, size_t depth, struct cxml_parser_frame *parent);
static void cxml_frame_appendchild(struct cxml_parser_frame *frame, struct cxml_parser_frame *child);
static void cxml_parser_finalizenodetree(struct cxml_parser_frame *frame,
                                         cxml_document *doc);
static void cxml_parser_freeframetree(struct cxml_parser_frame *frame);

void cxml_parser_traverseframetree(struct cxml_parser_frame *frame); //TODO REMOVE

cxml_parser *cxml_parser_new()
{
    CXML_DEBUG("Creating parser");
    cxml_parser *parser = malloc(sizeof *parser);
    parser->depth = 0;
    parser->error = CXML_ESUCCESS;
    parser->state = CXML_PS_RDY;
    parser->curframe = NULL;
    parser->rootframe = NULL;
    parser->bufsize = 0;
    parser->buf = NULL;
    return parser;
}

cxml_document *cxml_parsefile(cxml_parser *parser, const char *filename)
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
    //TODO finalize and return document


    CXML_DEBUG("Printing frame tree");
    cxml_parser_traverseframetree(parser->rootframe);

    cxml_document *doc = cxml_document_new();
    cxml_parser_finalizenodetree(parser->rootframe, doc);
    cxml_parser_freeframetree(parser->rootframe);

    free(parser->buf);
    parser->bufsize = 0;
    return doc;
}

cxml_document *cxml_parsexml(cxml_parser *parser, const char **xml, size_t lines)
{
    //TODO implement
    return NULL;
}

static void cxml_parsestring(cxml_parser *parser, const char *line)
{
    static int enterCount = 0;
    char *trimmedline = cxml_parser_trimwhitespace(line);
    size_t linelen = strlen(trimmedline);
    enum cxml_ptagtype tagtype;
    char *tag = NULL;
    char buf[256] = {0};
    unsigned offset = 0;
    for (size_t i = 0; i < linelen ; ++i) {

        if(trimmedline[i] == '<'){
            cxml_readtagname(linelen, &tag, trimmedline, &i, &tagtype);
            if(tagtype == CXML_PTT_CLOSING){
                CXML_DEBUGF("Offset: %d", offset);
                CXML_PARSER_WRITE_TO_STORE(parser, buf, offset);
                if(parser->bufsize > 0){
                    CXML_DEBUG("We made it");
                    parser->curframe->innerText = malloc(parser->bufsize+1);
                    strncpy(parser->curframe->innerText, parser->buf, parser->bufsize);
                    parser->curframe->innerText[parser->bufsize] = '\0';
                    free(parser->buf);
                    parser->buf = NULL;
                    parser->bufsize = 0;
                }

                parser->depth--;
                parser->curframe = parser->curframe->parent;
            }

            if(tagtype == CXML_PTT_OPENING || tagtype == CXML_PTT_OPENCLOSE){
                struct cxml_parser_frame *newFrame = cxml_frame_new(tag, parser->depth, parser->curframe);

                if(parser->depth == 0) parser->rootframe = newFrame;
                if(parser->curframe != NULL) cxml_frame_appendchild(parser->curframe, newFrame);

                if(tagtype == CXML_PTT_OPENING){
                    parser->curframe = newFrame;
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
    if(offset > 0){
        CXML_PARSER_WRITE_TO_STORE(parser, buf, offset);
    }
    free(tag);
    free(trimmedline);
}

/*Parser support functions*/
static char *cxml_readtagname(size_t len, char **tag, const char *line, size_t *loopinc, enum cxml_ptagtype *tagtype)
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
static struct cxml_parser_frame *cxml_frame_new(const char *tag, size_t depth, struct cxml_parser_frame *parent)
{
    struct cxml_parser_frame *frame = malloc(sizeof *frame);
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
    frame->innerText = NULL;
    frame->parent = parent;
    frame->depth = depth;
    return frame;
}

static void cxml_frame_appendchild(struct cxml_parser_frame *frame, struct cxml_parser_frame *child)
{
    struct cxml_parser_framelist *list = frame->children;
    struct cxml_parser_framelist *new = malloc(sizeof *new);
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
void cxml_parser_traverseframetree(struct cxml_parser_frame *frame)
{
    if(frame == NULL) return;

    cxml_stack *stack = cxml_stack_new();

    struct cxml_parser_frame *curframe;
    struct cxml_parser_framelist *curchild;
    cxml_stack_push(stack, frame);
    while(!cxml_stack_isempty(stack)){
        curframe = cxml_stack_pop(stack);
        curchild = curframe->children->next;

        while(curchild != NULL){
            cxml_stack_push(stack, curchild->frame);
            curchild = curchild->next;
        }

        CXML_DEBUGF("Tag: %s", curframe->tag);
        CXML_DEBUGF("Depth: %zu",curframe->depth);
        CXML_DEBUGF("Parent: %s", curframe->parent != NULL ? curframe->parent->tag : "none");
        CXML_DEBUGF("Text: %s", curframe->innerText != NULL ? curframe->innerText : "none");
    }

    cxml_stack_destroy(stack);
}

static void cxml_parser_freeframetree(struct cxml_parser_frame *frame)
{
    if(frame == NULL) return;
    cxml_stack *stack = cxml_stack_new();

    struct cxml_parser_frame *curframe;
    struct cxml_parser_framelist *curchild;
    struct cxml_parser_framelist *tmpfree;
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

        free(curframe->innerText);
        free(curframe->tag);
        free(curframe->children);
        free(curframe);
    }
    cxml_stack_destroy(stack);
}

static void cxml_parser_finalizenodetree(struct cxml_parser_frame *frame,
                                         cxml_document *doc)
{
    if(frame == NULL || doc == NULL) return;

    cxml_stack *stack = cxml_stack_new();

    struct cxml_parser_frame *curframe;
    struct cxml_parser_framelist *curchild;
    struct cxml_node *curparent;
    struct cxml_node *curnode;

    cxml_stack_push(stack, doc->docnode);
    cxml_stack_push(stack, frame);

    while(!cxml_stack_isempty(stack)){
        curframe = cxml_stack_pop(stack);
        curparent = cxml_stack_pop(stack);
        curchild = curframe->children->next;

        curnode = cxml_node_new(CXML_NODE, curframe->tag, curframe->innerText, curparent);
        if(curparent != NULL){
            if(curparent->type == CXML_DOCUMENT){
                cxml_document_setRoot(doc, curnode);
            }else {
                cxml_node_appendChild(curparent, curnode);
            }
        }

        while(curchild != NULL){
            cxml_stack_push(stack, curnode);
            cxml_stack_push(stack, curchild->frame);
            curchild = curchild->next;
        }
    }
    cxml_stack_destroy(stack);
}

