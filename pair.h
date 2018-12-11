
#ifndef _pair_
#define _pair_

#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef void (*PairFreeFunction) (void* elemAddr);

typedef struct
{
    void* first;
    void* second;
    int firstElemSize;
    int secondElemSize;
    PairFreeFunction firstFreeFn;
    PairFreeFunction secondFreeFn;
}
pair;

void PairNew (pair* p,
              void* first, void* second,
              int firstElemSize, int secondElemSize,
              PairFreeFunction firstFreeFn,
              PairFreeFunction secondFreeFn);
void PairDispose (pair* p);

void PairGetFirst (pair* p, void* firstAddr);
void PairGetSecond (pair* p, void* secondAddr);

void PairSetFirst(pair *p, void * addr);
void PairSetSecond(pair *p, void * addr);


#endif
