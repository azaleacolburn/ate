#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/**
 * Evaluates to the formatted buffer
 */
#define FMT(fmt, ...)                                                          \
    ({                                                                         \
        char* buf = NULL;                                                      \
                                                                               \
        int result = asprintf(&buf, fmt, __VA_ARGS__);                         \
        if (result == -1) {                                                    \
            perror("Could not asprintf");                                      \
            exit(1);                                                           \
        }                                                                      \
                                                                               \
        buf;                                                                   \
    })

bool prefixed_by(const char* prefix, const char* str);

/**
 * Reads a null terminated string into an integer.
 *
 * Stopping when a non-numerical digit is found.
 *
 * Returns `0` if `str` does not contain a valid integer
 */
uint32_t read_to_int(const char* str);
