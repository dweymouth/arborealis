#include <stdlib.h>
#include "hashtable.h"

#define MAX_LOAD_FACTOR 0.5f

typedef struct kv_pair {
	unsigned int key;
	unsigned int value;
} KV_Pair;

typedef struct hashtable {
	unsigned int size;
	unsigned int arraySize;
	KV_Pair* *array;
} Hashtable;

inline unsigned int hash(int i) {
	return i * 433; // prime
}

Hashtable *ht_create(unsigned int initialCapacity) {
	if (initialCapacity == 0) {
		initialCapacity = 17;
	}
	Hashtable *ht = malloc(sizeof(Hashtable));
	ht->size = 0;
	ht->arraySize = initialCapacity;
	ht->array = calloc(initialCapacity, sizeof(KV_Pair*));
	return ht;
}

// adds to array without doubling
void _add(KV_Pair* *array, unsigned int length, KV_Pair *kvPair) {
	unsigned int bucketIndex = hash(kvPair->key) % length;
	if (!array[bucketIndex]) {
		array[bucketIndex] = kvPair;
	} else {
		// linear probing - find next empty position
		while (array[bucketIndex = (bucketIndex + 1) % length] > 0);
		array[bucketIndex] = kvPair;
	}
}

void ht_add(Hashtable *ht, unsigned int key, unsigned int value) {
	unsigned int arraySize = ht->arraySize;
	float loadFactor = (float)ht->size / arraySize;
	if (loadFactor > MAX_LOAD_FACTOR) {
		KV_Pair* *array = ht->array;
		unsigned int newArraySize = arraySize * 2;
		KV_Pair* *newArray = calloc(newArraySize, sizeof(KV_Pair*));
		// copy over all kv pairs from old array
		unsigned int i;
		for (i = 0; i < arraySize; i++) {
			if (array[i]) {
				_add(newArray, newArraySize, array[i]); 
			}
		}
		free(array);
		ht->array = newArray;
		ht->arraySize = newArraySize;
	}
	KV_Pair *kvPair = malloc(sizeof(KV_Pair));
	kvPair->key = key;
	kvPair->value = value;
	_add(ht->array, ht->arraySize, kvPair);
	++ht->size;
}

unsigned int ht_lookup(Hashtable *ht, unsigned int key) {
	int length = ht->arraySize;
	int bucketIndex = hash(key) % length;
	int firstBucketIndex = bucketIndex;
	KV_Pair* *array = ht->array;
	do {
		if (array[bucketIndex] && array[bucketIndex]->key == key) {
			return array[bucketIndex]->value;
		}
		bucketIndex = (bucketIndex + 1) % length;
	} while (bucketIndex != firstBucketIndex);
	return -1; // not found
}

unsigned int ht_size(Hashtable *ht) {
	return ht->size;
}
