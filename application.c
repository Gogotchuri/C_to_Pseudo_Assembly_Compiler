#include <stdio.h>
#include "tokenizer.h"
//#include "parser.h"
#include "grammar.h"

void TokeTest(){
    tokenizer t;
    pair * pair_addr;
    const char* source_file = "source.txt";
    TokenizerNew(&t, source_file);
    while(TokenizerNextTok(&t)){
        TokenizerCurrent(&t, &pair_addr);
        char* first; PairGetFirst(pair_addr, &first);
        char* second; PairGetSecond(pair_addr, &second);
        printf("Token: %s -> Type: %s\n", first, second);
    }

    TokenizerDispose(&t);
}
int main ()
{
    grammar gr;
    GrammarNew(&gr);
    TokeTest();
    //const char* source_file = "source.txt";
    GrammarDispose(&gr);
    // parser p;
    // ParserNew(&p, source_file);
    // ParserDispose(&p);
    return 0;
}
