#include "fs.h"
#include "str.h"
#include "svec.h"
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char* read_link(const char* link_path) {
    const size_t buf_size = 1024 * sizeof(char);
    char* const link_result = malloc(buf_size);
    const ssize_t len = readlink(link_path, link_result, buf_size - 1);

    if (len == -1) {
        perror("Could not read HEAD path");
        exit(1);
    }

    link_result[len] = '\0';
    return link_result;
}

char* read_to_string(const char* path) {
    FILE* fp = fopen(path, "r");
    if (!fp) {
        puts(path);
        perror("Failed to open file");
        unlink(path);
        exit(1);
    }

    return read_from_fp_to_string(fp);
}

char* read_from_fp_to_string(FILE* fp) {
    if (fseek(fp, 0L, SEEK_END) != 0) {
        fclose(fp);
        exit(1);
    }

    size_t file_size = ftell(fp);

    if (fseek(fp, 0L, SEEK_SET) != 0) {
        perror("Failed to rewind file");
        exit(1);
    }

    char* buffer = malloc(sizeof(char) * (file_size + 1));
    if (buffer == NULL) {
        perror("Failed to allocate memory for buffer");
        exit(1);
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, fp);

    if (bytes_read < file_size && ferror(fp)) {
        perror("Failed to read file");
        free(buffer);
        fclose(fp);
        exit(1);
    }

    buffer[bytes_read] = '\0';
    fclose(fp);

    return buffer;
}

void create_dir(const char* path) {
    int result = mkdir(path, 0777);
    if (result != 0) {
        perror("Could not create folder");
        exit(1);
    }
}

FILE* create_file(const char* path) {
    FILE* fp = fopen(path, "w+");

    if (fp == NULL) {
        puts(path);
        perror("Could not create new file");
        exit(1);
    }

    return fp;
}

FILE* create_file_rec(char* path) {
    int path_len = strlen(path);
    if (path[path_len - 1] == '/') {
        path[path_len - 1] = 0;
    }

    for (int i = path_len - 1; i > 0; i--) {
        if (path[i] == '/') {
            path[i] = 0;

            char* parent_name = path;
            char* file_name = path + i - 1;

            int res = create_dir_recursive(parent_name);
            if (res != 0) {
                return NULL;
            };

            return create_file(file_name);
        }
    }

    return create_file(path);
}

SizeVec read_file_split_by_line(const char* path) {
    char* contents = read_to_string(path);
    char* curr = contents;

    SizeVec output;
    svec_init(&output, 20);

    if (strcmp(contents, "") == 0 || strcmp(contents, "\n") == 0) {
        return output;
    }

    svec_push(&output, curr);

    while (*curr != '\0') {
        if (*curr == '\n' && *(curr + 1) != EOF) {
            /* Copy pointer of next line into the `SplitResult` */
            svec_push(&output, curr + 1);
            /* Replace newline character with null terminator */
            *curr = '\0';
        }

        curr++;
    }

    return output;
}

void free_split_result(SizeVec output) {
    // All lines point to the same underlying buffer,
    // so we only have to free the first one
    if (output.arr != NULL && output.len > 0)
        free(output.arr[0]);
}

/* Make a directory; already existing dir okay */
static int maybe_create_dir(const char* path, mode_t mode) {
    struct stat st;
    errno = 0;

    /* Try to make the directory */
    if (mkdir(path, mode) == 0)
        return 0;

    /* If it fails for any reason but EEXIST, fail */
    if (errno != EEXIST)
        return -1;

    /* Check if the existing path is a directory */
    if (stat(path, &st) != 0)
        return -1;

    /* If not, fail with ENOTDIR */
    if (!S_ISDIR(st.st_mode)) {
        errno = ENOTDIR;
        return -1;
    }

    errno = 0;
    return 0;
}

/* Adapted from http://stackoverflow.com/a/2336245/119527 */
int create_dir_recursive(const char* path) {
    char* path_mut = NULL;
    char* p;
    int result = -1;
    mode_t mode = 0777;

    errno = 0;

    /* Copy string so it's mutable */
    path_mut = strdup(path);
    if (path_mut == NULL)
        goto out;

    /* Iterate the string */
    for (p = path_mut + 1; *p; p++) {
        if (*p == '/') {
            /* Temporarily truncate */
            *p = '\0';

            if (maybe_create_dir(path_mut, mode) != 0)
                goto out;

            *p = '/';
        }
    }

    if (maybe_create_dir(path_mut, mode) != 0)
        goto out;

    result = 0;

out:
    free(path_mut);
    return result;
}

bool streams_are_equal(FILE* a, FILE* b) {
    const size_t chunk_size = 256;

    char a_buf[chunk_size];
    char b_buf[chunk_size];

    memset(a_buf, 0, chunk_size);
    memset(b_buf, 0, chunk_size);

    do {
        char* const a_res = fgets(a_buf, chunk_size, a);
        char* const b_res = fgets(b_buf, chunk_size, b);

        /**
         * If we've reached the point where the fiLes can't be read further
         * without finding differences, they must be the same
         */
        if (a_res == NULL && b_res == NULL)
            return true;
    } while (memcmp(a_buf, b_buf, chunk_size) == 0);

    return false;
}

bool write_stream_to_stream(FILE* src, FILE* dest) {
    if (src == NULL || dest == NULL)
        return false;

    const size_t chunk_size = 256;
    char buf[chunk_size];

    size_t bytes_read;
    while ((bytes_read = fread(buf, sizeof(char), chunk_size, src)) > 0) {
        const size_t bytes_written =
            fwrite(buf, sizeof(char), bytes_read, dest);

        if (bytes_read < bytes_written) {
            /* Write error occured */
            perror("Could not write new commit file");
            exit(1);
        }
    }

    return true;
}

void copy_dir_rec(const char* src, const char* dest, SizeVec* added) {
    DIR* const src_dir = opendir(src);
    if (src_dir == NULL)
        return;
    const struct dirent* src_entry;

    while ((src_entry = readdir(src_dir)) != NULL) {
        const char* file_name = src_entry->d_name;

        if (!svec_contains_str(added, file_name))
            continue;

        char* dest_file_path = FMT("%s/%s", dest, file_name);
        FILE* dest_fp = fopen(dest_file_path, "w");

        char* src_file_path = FMT("%s/%s", src, file_name);
        create_dir_recursive(src_file_path);
        FILE* src_fp = fopen(dest_file_path, "w");

        if (S_ISDIR(dest_fp->_mode)) {
            copy_dir_rec(dest_file_path, src_file_path, added);
            continue;
        }

        const bool success = write_stream_to_stream(src_fp, dest_fp);
        if (!success)
            printf("Failed to write %s to %s", src_file_path, dest_file_path);
    }
}
