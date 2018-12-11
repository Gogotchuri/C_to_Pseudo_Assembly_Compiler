#ifndef code_parser
#define code_parser


#include "tokenizer.h"
#include "grammar.h"
#include "bool.h"
#include "node.h"

typedef struct{
    tokenizer tok;
    grammar gram;
    vector functions_ASTs;
} parser;

void ParserNew(parser * p, const char * source_file);
void ParserDispose(parser * p);

#endif