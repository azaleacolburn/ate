#pragma once

#include <stdlib.h>

typedef struct Bucket {
    struct Bucket* next;
    /* Heap allocated */
    char* key;
    /* Heap allocated construct of `item_size` */
    void* value;
} Bucket;

typedef struct {
    Bucket** buckets;
    size_t len;
    size_t item_size;
} HashMap;

void hashmap_init(HashMap* map, size_t len, size_t item_size);

void* hashmap_get(HashMap* map, char* key);

/* The hashmap will copy `value` onto the heap */
void hashmap_set(HashMap* map, char* key, void* value);

void hashmap_remove(HashMap* map, char* key);
