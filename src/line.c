#include "line.h"
#include "buffer.h"
#include <stdlib.h>
#include <string.h>

void free_lines(Line* line) {
    free_lines(line->next);

    free(line->contents);
    free(line);
}

Line* new_line(LineStatus status, Line* next, Line* prev) {
    Line* line = malloc(sizeof(Line));

    line->prev = prev;
    line->next = next;

    line->contents = malloc(INITIAL_LINE_ALLOCATION);
    line->capacity = INITIAL_LINE_ALLOCATION;

    line->modified = status;

    return line;
}

Line* new_line_no_contents(LineStatus status, Line* next, Line* prev) {
    Line* line = malloc(sizeof(Line));

    line->prev = prev;
    line->next = next;

    line->contents = NULL;
    line->capacity = 0;

    line->modified = status;

    return line;
}

void insert_into_line(Line* line, char* contents, size_t idx) {
    size_t original_content_len = strlen(line->contents);
    size_t inserted_content_len = strlen(contents);

    if (original_content_len <= idx)
        return;

    // Reserve more space in memory for the inserted content
    if (original_content_len <= line->capacity + inserted_content_len) {
        line->capacity += inserted_content_len * 2;
        line->contents = realloc(line->contents, line->capacity);
    }

    // Shift over the displaced contents in memory
    (void)memmove(line->contents + idx + inserted_content_len,
                  line->contents + idx, original_content_len - idx);

    // Write the inserted contents to memory
    (void)memcpy(line->contents + idx, contents, inserted_content_len);
}

// Probably not a good function from an API design perspective
void append_to_line(Line* line, char* contents) {
    size_t original_content_len = strlen(line->contents);
    size_t inserted_content_len = strlen(contents);

    // Reserve more space in memory for the inserted content
    if (original_content_len <= line->capacity + inserted_content_len) {
        line->capacity += inserted_content_len * 2;
        line->contents = realloc(line->contents, line->capacity);
    }

    // Write the appended contents to memory
    (void)memcpy(line->contents + original_content_len, contents,
                 inserted_content_len);
}
