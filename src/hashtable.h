#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct hashtable Hashtable;

/**
 * Creates a hashtable with the specified initial capacity.
 * Specifying 0 lets the implementation choose a default initial capacity.
 */
Hashtable *ht_create(unsigned int);

/**
 * Adds the given key-value pair to the hashtable. The implementation does
 * not deal with adding the same key twice. If this is done, currently the
 * lookup will always return the first binding for the key.
 */
void ht_add(Hashtable *, unsigned int key, unsigned int value);

/**
 * Returns the value bound to the given key, or -1 (MAX_INT)
 * if the key is not bound.
 */
unsigned int ht_lookup(Hashtable *, unsigned int key);

/**
 * Returns the number of bindings contained in the hashtable.
 */
unsigned int ht_size(Hashtable *);

#endif
