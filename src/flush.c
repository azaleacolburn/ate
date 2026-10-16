#include "buffer.h"
#include <ncurses.h>

void setup() { initscr(); }

void flush(Buffer* buf) {
    move(0, 0);
    clear();

    for (size_t i = 0; i < buf->line_count; i++) {
        mvprintw(i, 0, "%s", itoa(i));
    }

    while (buf->lines != NULL) {
        addstr(buf->lines->contents);
        buf->lines = buf->lines->next;
    }

    refresh();
}
