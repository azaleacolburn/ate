#include "editor_io.h"
#include "fs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* tmp_file_fmt_string = "/tmp/vim_bufferXXXXXX";

void create_tmp_vim_file(char file_name_buffer[]) {
    strcpy(file_name_buffer, tmp_file_fmt_string);
    int fd = mkstemp(file_name_buffer);

    if (fd == -1) {
        perror("Failed to create temporary file");
        exit(1);
    }

    close(fd);
}

char* get_editor_output() {
    char filename[strlen(tmp_file_fmt_string)];
    create_tmp_vim_file(filename);

    char command[256];
    snprintf(command, sizeof(command), "vim %s", filename);

    printf("Opening Vim... Write you commit message in that buffer, then save "
           "and exit (:wq)");
    sleep(1);

    int status = system(command);
    if (status == -1) {
        fprintf(stderr, "Vim encounterred an error or was interrupted");
        unlink(filename);
        exit(1);
    }

    char* contents = read_to_string(filename);

    printf("\n--- Captured Buffer Output ---\n");
    printf("%s", contents);
    printf("\n------------------------------\n");

    return contents;
}
