#include "buffer.h"
#include "line.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX(a, b) a > b ? a : b

static void move_forward_one_line(FILE* fp) {
    int c;
    // Read and discard characters until a newline or end-of-file is found
    do {
        c = fgetc(fp);
    } while (c != '\n' && c != EOF);
}

Line* last_node(Buffer* buf) {
    Line* tail = buf->first_line;

    while (tail->next != NULL)
        tail = tail->next;

    return tail;
}

void shift_bounds_back(Buffer* buf) {
    // If we already have the line loaded, just move the `first_line` pointer
    if (buf->first_line->prev != NULL) {
        buf->first_line = buf->first_line->prev;
        return;
    }

    assert(buf->first_line == buf->head);

    if (buf->buffer_start == 0)
        return;

    fseek(buf->fp, buf->buffer_start, SEEK_SET);

    // Read backwards one character at a time,
    // looking for the beginning of the current line
    char ch;
    do
        (void)fseek(buf->fp, -2, SEEK_CUR);
    while ((ch = fgetc(buf->fp)) != '\n');

    // We're on the character after a `\n`, so the start of a new line
    buf->buffer_start = ftell(buf->fp);

    Line* line = new_line(LINE_CLEAN, buf->head, NULL);
    buf->first_line = line;

    size_t size = 0;
    (void)getline(&line->contents, &size, buf->fp);
    line->capacity = size;
}

void shift_bounds_forward(Buffer* buf) {
    // Move to the last line that is currently displayed on the screen
    // And if the number of lines displayed is less than the screen size
    // return from the entire function.
    Line* last_line = buf->first_line;
    for (size_t i = 0; i < buf->screen_size; i++) {
        last_line = last_line->next;

        if (last_line == NULL)
            return;
    }

    // If there is no next line, load it!
    if (last_line->next == NULL) {
        // Move file to buffer offset
        (void)fseek(buf->fp, buf->buffer_start, SEEK_SET);

        // For each line in the buffer, move forward one line in the file
        Line* node = buf->head;
        while (node != last_line) {
            move_forward_one_line(buf->fp);
            node = node->next;
        }

        // Create the new line at the end of the buffer
        Line* line = new_line(LINE_CLEAN, NULL, last_line);
        size_t size = 0;
        __ssize_t result = getline(&line->contents, &size, buf->fp);
        line->capacity = size;

        // If there's no next line in the file, abort
        if (result == -1) {
            free(line->contents);
            free(line);
            return;
        }
    }

    // Move the first line down one to include the next line
    buf->first_line = buf->first_line->next;
}

size_t count_lines_loaded(Buffer* buf, Line* last_line) {
    size_t lines_loaded = 0;

    Line* node = buf->head;
    while (node != last_line) {
        lines_loaded++;
        node = node->next;
    }

    return lines_loaded;
}

void load_buffer_at(Buffer* buf, size_t offset) {
    // Free old lines
    while (buf->head != NULL) {
        Line* next = buf->head->next;

        free(buf->head->contents);
        free(buf->head);

        buf->head = next;
    }

    if (fseek(buf->fp, offset, SEEK_SET) != 0)
        return;

    Line* prev = NULL;
    Line* curr = NULL;
    size_t size = 0;

    for (size_t i = 0; i < buf->screen_size; i++) {
        curr = malloc(sizeof(Line));
        curr->modified = LINE_CLEAN;

        // Read the line
        (void)getline(&curr->contents, &size, buf->fp);
        curr->capacity = size;

        // Set current and next pointers
        curr->next = NULL;
        curr->prev = prev;
        if (prev != NULL)
            prev->next = curr;

        prev = curr;
    }

    while (prev->prev != NULL)
        prev = prev->prev;

    buf->head = prev;
    buf->first_line = prev;
    buf->buffer_start = offset;
}
