#pragma once

#include "line.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    // The underlying file of the buffer
    FILE* fp;

    // The character index in `fp` of the start of the first line loaded by this
    // buffer
    //
    // `head` should represent this position in the file
    size_t buffer_start;

    // Number of lines to render
    size_t screen_size;

    // Lines in the buffer to be displayed
    // The first one is the `lower`est line in the file
    Line* first_line;

    // The first line loaded in the current contiguous set of lines
    //
    // This is the root line which is held to prevent loosing access
    // to loaded lines that lay outside the scope
    Line* head;
} Buffer;

typedef struct {
} GlobalState;

void shift_bounds_back(Buffer* buf);

void shift_bounds_forward(Buffer* buf);

size_t count_lines_loaded(Buffer* buf, Line* last_line);

/*
 * Loads the contents of the file into the line list in `buf`
 * from character `buf->lower` to character `buf->upper`
 */
void load_buffer(Buffer* buf);
