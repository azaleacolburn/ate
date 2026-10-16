#include <stdbool.h>
#include <stdint.h>
#include <string.h>

bool prefixed_by(const char* prefix, const char* str) {
    return strncmp(prefix, str, strlen(prefix)) == 0;
}

uint32_t read_to_int(const char* str) {
    uint32_t num = 0;

    while (str != NULL && *str >= '0' && *str <= '9') {
        num *= 10;
        num += *str - '0';
    }

    return num;
}
