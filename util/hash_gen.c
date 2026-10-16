#include "hash_gen.h"
#include "djb2.h"
#include <stdio.h>

void subcommand_hash_gen() {
    const char* subcommands[] = {"init", "add", "commit", "checkout", 0};

    for (const char** ptr = subcommands; *ptr != 0; ptr++) {
        unsigned long hash = djb2_hash((unsigned char*)*ptr);
        printf("Hashed %s: %lu\n", *ptr, hash);
    }
}
