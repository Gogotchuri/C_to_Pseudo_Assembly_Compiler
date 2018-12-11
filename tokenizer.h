
#ifndef _tokenizer_
#define _tokenizer_

/* This Tokenizer serves simple but humble purpose. It is precisly slicing given code
 * into tokens and decides what type each token is, It doesn't check if code violates any
 * syntax rules, this job is left for parser and grammar;
 * */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "tok_checker.h"
#include "compilatorConstants.h"
#include "vector.h"
#include "hashset.h"
#include "pair.h"

/*Currently known bugs: 1)problem with deducing pointers
                        2)problem with deducing strings and chars without white spaces*/

typedef struct __tokenizer__
{
    int current_index;
    vector basic_tokens;
    vector token_to_type;
    hashset typesMap;
}
tokenizer;

/* Function: TokenizerNew
 * ---------------------------------------------
 * Creates new tokenizer from given filename
 * */
void TokenizerNew (tokenizer* t, const char * source_file);

/*Returns true if tokenizer have next token*/
bool TokenizerHaveMore(tokenizer* t);

/*Writes next tokens addr to dest. have same result as calling: TokenizerPeekNth(t, 1, &pair_addr)*/
bool TokenizerPeekNext(tokenizer* t, pair **dest);

/*Writes previous tokens addr to dest. have same result as calling: TokenizerPeekNth(t, -1, &pair_addr)*/
bool TokenizerPeekPrev(tokenizer* t, pair **dest);

/*Writes current tokens addr to dest. have same result as calling: TokenizerPeekNth(t, 0, &pair_addr)*/
bool TokenizerCurrent(tokenizer* t, pair **dest);

/*Peeks Nth token, assumes current token counter is origin. if Nth token exists returns true
    and writes addr of the token to dest parameter;
    If Nth token is out of range returns false and doesn't touch dest;
    
    Usage Example: TokenizerPeekNth(t, 0, &pair_addr) writes current tokens address in pair_addr
    Usage Example: TokenizerPeekNth(t, -1, &pair_addr) writes previous tokens address in pair_addr
    Usage Example: TokenizerPeekNth(t, 1, &pair_addr) writes next tokens address in pair_addr*/
bool TokenizerPeekNth(tokenizer* t, int n, pair **dest);

/*Moves token counter forward if possible and returns true;
    If you are standing on the last token before invoking this function, 
    function returns false.
    There is not current token before calling this method!
    After this function returns true, you can get current token address
    from function: TokenizerCurrent*/
bool TokenizerNextTok(tokenizer* t);

/*Tokenizer Dispose
    Warning: disposing tokenizer may result in deleting token pair allocations
        it is best to be done last*/
void TokenizerDispose (tokenizer* t);

//-------------helpers needed in other classes---------------------
/* Hashes pair stucture instance by first element (which should be string)
 * If first elem is anything other than string behaviour is undefined*/
int PairHashFnByFirstString (const void* vpairAddr, int numBuckets);

/* Compares pair stucture instance by first element (which should be string)
 * If first elem is anything other than string behaviour is undefined*/
int PairCmpFnByFirstString (const void *vpairAddr1, const void *vpairAddr2);
/*Just calls pair dispose function*/
void PairFreeFn (void *vpairAddr);

/*Basically substr but defies procedural paradigm principles which should be eliminated*/
char* GetStringBetween (int startIndex, int finishIndex, char* str);

#endif
