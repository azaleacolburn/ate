#include "string_builder.h"
#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void string_builder_init(StringBuilder* builder, size_t initial_capacity) {
    char* buffer = malloc(initial_capacity);
    if (buffer == NULL) {
        perror("Failed to allocate memory for string builder");
        exit(1);
    }

    string_builder_init_with_buf(builder, initial_capacity, buffer);
}

void string_builder_init_with_buf(StringBuilder* builder,
                                  size_t initial_capacity, char* buffer) {
    builder->cap = initial_capacity;
    builder->len = 0;

    builder->buf = buffer;
    memset(builder->buf, 0, initial_capacity);
}

void string_builder_push(StringBuilder* builder, char* str) {
    size_t idx = builder->len;
    builder->len += strlen(str);

    if (builder->len > builder->cap) {
        while (builder->len >= builder->cap)
            builder->cap *= 2;
        builder->buf = realloc(builder->buf, builder->cap);
    }

    strcpy(builder->buf + idx, str);
}

void string_builder_pushln(StringBuilder* builder, char* str) {
    string_builder_push(builder, str);
    string_builder_push(builder, "\n");
}

char string_builder_idx(StringBuilder* builder, size_t idx) {
    return builder->buf[idx];
}

void free_string_builder(StringBuilder builder) { free(builder.buf); }
