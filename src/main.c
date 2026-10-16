#include "buffer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum Mode {
    MODE_NORMAL,
    MODE_INSERT,
    MODE_VISUAL,
    MODE_META,
    MODE_COUNT,
} Mode;

typedef struct {
    char* filename;
    Mode mode;

    Buffer* buffer;
} State;

typedef struct {
    executor
} Command;

Command commands[256];

State state;

int main(int argc, char** argv) {
    assert(sizeof(Line) < 64);

    state.filename = (argc == 2) ? argv[1] : NULL;
    state.buffer = malloc(sizeof(Buffer));

    if (state.filename != NULL) {
        state.buffer->fp = fopen(state.filename, "w+");
        if (state.buffer->fp == NULL) {
            fprintf(stderr, "Could not open file %s\n", state.filename);
            exit(1);
        }
    }

    while (true) {
        int ch = getchar();
        if (ch == -1)
            break;

        switch (ch) {
            '' }
    }
}
