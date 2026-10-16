#include "hashmap.h"
#include "djb2.h"
#include <stdio.h>
#include <string.h>

void hashmap_init(HashMap* map, size_t len, size_t item_size) {
    map->len = len;
    map->item_size = item_size;

    size_t buffer_size = len * sizeof(Bucket*);

    map->buckets = malloc(buffer_size);
    memset(map->buckets, 0, buffer_size);
}

/**
 * Returns a pointer to a linked-list of buckets
 *
 * Not a 2d array of buckets
 */
static Bucket** get_ref_to_bucket_chain(HashMap* map, char* key) {
    size_t hash = djb2_hash((unsigned char*)key);
    size_t idx = hash % map->len;

    return map->buckets + idx;
}

void* hashmap_get(HashMap* map, char* key) {
    Bucket* bucket = *get_ref_to_bucket_chain(map, key);
    void* value = NULL;

    while (bucket != NULL) {
        if (strcmp(bucket->key, key) == 0) {
            value = bucket->value;
            break;
        }

        bucket = bucket->next;
    }

    return value;
}

/**
 * It is the responsibility of the caller to remove the bucket from the list
 */
static void free_bucket(Bucket* bucket) {
    free(bucket->value);
    free(bucket->key);
    free(bucket);
}

void hashmap_remove(HashMap* map, char* key) {
    /**
     * Pointer to a pointer to the parent
     *
     * This points to the value that needs to be set to `bucket->next`
     * if the parent is removed
     */
    Bucket** bucket_ptr = get_ref_to_bucket_chain(map, key);
    Bucket* bucket = NULL;

    while ((bucket = *bucket_ptr) != NULL) {
        if (strcmp(bucket->key, key) == 0) {
            *bucket_ptr = bucket->next;
            free_bucket(bucket);
            return;
        }

        bucket_ptr = &(*bucket_ptr)->next;
    }
}

static Bucket* new_bucket(char* key, void* value, size_t item_size) {
    Bucket* bucket = malloc(sizeof(Bucket));
    bucket->next = NULL;

    bucket->key = malloc(strlen(key) + 1);
    strcpy(bucket->key, key);

    bucket->value = malloc(item_size);
    memcpy(bucket->value, value, item_size);

    return bucket;
}

/**
 * You'll note some similarities between `hashmap_remove` and `hashmap_set`
 * Feel free to extract their shared behavior, but the iter functions
 * Need different parameters, so it's a bit awkward.
 */
void hashmap_set(HashMap* map, char* key, void* value) {
    /**
     * We need a pointer to a pointer to a bucket to handle the case where
     * no bucket exists with for key.
     */
    Bucket** bucket_chain = get_ref_to_bucket_chain(map, key);

    /* Iterate over bucket list, looking for bucket with value*/
    Bucket* bucket = NULL;
    while ((bucket = *bucket_chain) != NULL) {
        if (strcmp(bucket->key, key) == 0) {
            /* A bucket exists with the same key value */
            memcpy(bucket->value, value, map->item_size);

            return;
        }

        bucket_chain = &(*bucket_chain)->next;
    }

    /* At end of entry list */
    *bucket_chain = new_bucket(key, value, map->item_size);
}
