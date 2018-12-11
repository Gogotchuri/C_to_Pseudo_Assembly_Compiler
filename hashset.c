
#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
	assert(numBuckets > 0);
	assert(hashfn != NULL);
	assert(comparefn != NULL);

	h->buckets = malloc(numBuckets * sizeof(vector));
	assert (h->buckets != NULL);

	h->numBuckets = numBuckets;
	h->numElems = 0;
	h->hashFn = hashfn;
	h->compareFn = comparefn;

	for (int bucket_i = 0; bucket_i < numBuckets; bucket_i ++)
	{
		void *curr_bucket_addr = (char *)h->buckets + bucket_i * sizeof(vector);
		VectorNew((vector *)curr_bucket_addr, elemSize, freefn, 0);
	}
}

void HashSetDispose(hashset *h)
{
	for (int bucket_i = 0; bucket_i < h->numBuckets; bucket_i ++)
	{
		void *curr_bucket_addr = (char *)h->buckets + bucket_i * sizeof(vector);
		VectorDispose((vector *)curr_bucket_addr);
	}
	free(h->buckets);
}

int HashSetCount(const hashset *h)
{
	return h->numElems;
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{
	for (int bucket_i = 0; bucket_i < h->numBuckets; bucket_i ++)
	{
		void *curr_bucket_addr = (char *)h->buckets + bucket_i * sizeof(vector);
		VectorMap((vector *)curr_bucket_addr, mapfn, auxData);
	}
}

void HashSetEnter(hashset *h, const void *elemAddr)
{
	vector *bucket = HashSetFindBucket(h, elemAddr);
	int position = VectorSearch(bucket, elemAddr, h->compareFn, 0, false);
	if (position != -1) { VectorReplace(bucket, elemAddr, position); }
	else
	{
		VectorAppend(bucket, elemAddr);
		h->numElems ++;
	}
}

void *HashSetLookup(const hashset *h, const void *elemAddr)
{
	vector *bucket = HashSetFindBucket(h, elemAddr);
	int position = VectorSearch(bucket, elemAddr, h->compareFn, 0, false);
	if (position != -1) { return VectorNth(bucket, position); }
	return NULL;
}

vector *HashSetFindBucket(const hashset *h, const void *elemAddr)
{
	assert(elemAddr != NULL);
	int hash = h->hashFn(elemAddr, h->numBuckets);
	assert(hash > -1 && hash < h->numBuckets);
	void *bucket = (char *)h->buckets + hash * sizeof(vector);
	return (vector *)bucket;
}
