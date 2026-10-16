#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void create_tmp_vim_file(char file_name_buffer[]);

// # Returns
// The heap allocated contents provided by the user through their
// It is the responsibility of the caller to free this memory
char* get_editor_output();
