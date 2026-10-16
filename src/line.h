#pragma once

#include <stdint.h>
#include <stdlib.h>

#define INITIAL_LINE_ALLOCATION 12

typedef enum : uint8_t {
    LINE_NEW,
    LINE_MODIFIED,
    LINE_CLEAN,
    LINE_STATUS_COUNT
} LineStatus;

// We want this to fit in a cache line really badly
typedef struct Line {
    char* contents;
    size_t capacity;

    struct Line* next;
    struct Line* prev;

    LineStatus modified;
} Line;

void free_lines(Line* line);

Line* new_line(LineStatus status, Line* next, Line* prev);

void push_to_line(Line* line, char* contents);

void insert_into_line(Line* line, char* contents, size_t idx);
