#pragma once

#include <stddef.h>

/**
 * The standard for `StringBuilder`s is to keep them stack allocated
 * and copy them between frames when necesssary.
 */
typedef struct {
    char* buf;
    size_t cap;
    size_t len;
} StringBuilder;

void string_builder_init(StringBuilder* builder, size_t initial_capacity);

void string_builder_init_with_buf(StringBuilder* builder,
                                  size_t initial_capacity, char* buffer);
// # SAFETY
// Calling this function on a `StringBuilder` that was initialized with a stack
// buffer is undefined behavior
void string_builder_push(StringBuilder* builder, char* str);

// # SAFETY
// Calling this function on a `StringBuilder` that was initialized with a stack
// buffer is undefined behavior
void string_builder_pushln(StringBuilder* builder, char* str);

char string_builder_idx(StringBuilder* builder, size_t idx);

void free_string_builder(StringBuilder builder);
