
#include "pair.h"

void PairNew (pair* p,
              void* first, void* second,
              int firstElemSize, int secondElemSize,
              PairFreeFunction firstFreeFn,
              PairFreeFunction secondFreeFn)
{
    p->first = malloc(firstElemSize);
    assert(p->first != NULL);

    p->second = malloc(secondElemSize);
    assert(p->second != NULL);

    memcpy(p->first, first, firstElemSize);
    memcpy(p->second, second, secondElemSize);

    p->firstElemSize = firstElemSize;
    p->secondElemSize = secondElemSize;
    p->firstFreeFn = firstFreeFn;
    p->secondFreeFn = secondFreeFn;
}

void PairDispose (pair* p)
{
    if (p->firstFreeFn != NULL)
        p->firstFreeFn(p->first);

    if (p->secondFreeFn != NULL)
        p->secondFreeFn(p->second);

    free(p->first);
    free(p->second);
}

void PairGetFirst (pair* p, void* firstAddr)
{
    memcpy(firstAddr, p->first, p->firstElemSize);
}

void PairGetSecond (pair* p, void* secondAddr)
{
    memcpy(secondAddr, p->second, p->secondElemSize);
}

void PairSetFirst(pair *p, void * addr){
    if (p->firstFreeFn != NULL)
        p->firstFreeFn(p->first);
    
    memcpy(p->first, addr, p->firstElemSize);
}
void PairSetSecond(pair *p, void * addr){
     if (p->secondFreeFn != NULL)
        p->secondFreeFn(p->second);
    
    memcpy(p->second, addr, p->secondElemSize);    
}
