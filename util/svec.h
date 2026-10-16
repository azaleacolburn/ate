#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef void* SizeVecItem;

typedef struct {
    SizeVecItem* arr;
    size_t len;
    size_t cap;
} SizeVec;

void svec_init(SizeVec* vec, size_t init_cap);

void svec_push(SizeVec* vec, SizeVecItem item);

SizeVecItem svec_get(SizeVec* vec, size_t idx);

void svec_free(const SizeVec vec,
               const void (*const item_pointee_destructor)(const SizeVecItem));

bool svec_contains_str(const SizeVec* const vec, const char* const str);
