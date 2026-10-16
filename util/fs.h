#pragma once

#include "svec.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Result is heap-allocated
 */
char* read_link(const char* link_path);

/**
 * The return value points onto the heap,
 * it is the responsibility of the caller to free that buffer.
 */
char* read_to_string(const char* path);

/**
 * `fp` must not be `NULL`
 */
char* read_from_fp_to_string(FILE* fp);

void create_dir(const char* path);

int create_dir_recursive(const char* path);

/**
 * It is the responsibility of the caller to free
 * the returned `FILE*` with `fclose`.
 */
FILE* create_file(const char* path);

/**
 * Will modify the `path` argument.
 *
 * It is the responsibility of the caller to free
 * the returned `FILE*` with `fclose`.
 */
FILE* create_file_rec(char* path);

// The caller is responsible for freeing only the first line,
// which will free the entire buffer
//
// Writing past the newline character of one line
// will write into the start of the next
//
// The only memory allocation this function makes is a single buffer
// into which the entire file is read
SizeVec read_file_split_by_line(const char* path);

// If `output->len` or `output->arr` have been modified,
// calling this function will result in undefined behavior
void free_split_result(SizeVec output);

/**
 * Returns whether the file streams have the same contents.
 * Consumes the contents of each stream.
 *
 * If both streams are invalid, or are invalid after a specific point
 * (before which they are equal in contents)
 * then this function considers them equal.
 */
bool streams_are_equal(FILE* a, FILE* b);

/**
 * Writes the contents of `src` to `dest`.
 *
 * If either pointer is `NULL`, nothing happens.
 */
bool write_stream_to_stream(FILE* src, FILE* dest);

/**
 * Writes every file and directory recursively from `src` into `dest`
 */
void copy_dir_rec(const char* src, const char* dest, SizeVec* added);
