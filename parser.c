#include "parser.h"

/*Prototypes*/

//------------------------Parser functionality---------------------------
void ParserNew(parser *p, const char *source_file){
    GrammarNew(&(p->gram));
    TokenizerNew(&(p->tok), source_file);
}

void ParserDispose(parser *p){
    TokenizerDispose(&(p->tok));
    GrammarDispose(&(p->gram));
}


//-------------------------------Helpers------------------------------------
