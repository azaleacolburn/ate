#include "svec.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void svec_realloc(SizeVec* vec) {
    vec->cap *= 2;

    SizeVecItem* new_arr = realloc(vec->arr, vec->cap);
    if (new_arr == NULL) {
        free(new_arr);
        perror("Could not reallocate `SizeVec`");
        exit(1);
    }

    vec->arr = new_arr;
}

/* PUBLIC API */

void svec_init(SizeVec* vec, size_t init_cap) {
    vec->len = 0;
    vec->cap = init_cap;

    vec->arr = (SizeVecItem*)malloc(sizeof(SizeVecItem) * vec->cap);
    if (vec->arr == NULL) {
        perror("Could not allocate buffer for `SizeVec`");
        exit(1);
    }
}

void svec_push(SizeVec* vec, SizeVecItem item) {
    if (vec->len >= vec->cap)
        svec_realloc(vec);

    vec->arr[vec->len++] = item;
}

SizeVecItem svec_get(SizeVec* vec, size_t idx) { return vec->arr[idx]; }

void svec_free(const SizeVec vec,
               const void (*const item_pointee_destructor)(const SizeVecItem)) {
    if (item_pointee_destructor != NULL)
        for (int i = 0; i < vec.len; i++)
            item_pointee_destructor(vec.arr[i]);

    free(vec.arr);
}

bool svec_contains_str(const SizeVec* const vec, const char* const str) {
    for (int i = 0; i < vec->len; i++) {
        char* other = vec->arr[i];
        if (strcmp(str, other) == 0) {
            return true;
        }
    }

    return false;
}
