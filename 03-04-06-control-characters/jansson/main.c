#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "build/jansson/include/jansson.h"

int run(const char* buf, size_t size, char** out_buf, size_t* out_size)
{
    json_t* json;
    json_error_t error;
    if (NULL == (json = json_loadb(buf, size, 0, &error))) {
        return -1;
    }

    size_t required_size =
        json_dumpb(json, NULL, 0, JSON_COMPACT); // determine required size of buffer
    if (required_size == 0) {
        json_decref(json);
        return -1;
    }

    char* write_buffer; // create buffer
    if (NULL == (write_buffer = (char*)malloc(required_size + 1))) {
        json_decref(json);
        return -1;
    }

    // write json string to buffer
    int bytes_written = json_dumpb(json, write_buffer, required_size, JSON_COMPACT);
    if (bytes_written == 0) {
        json_decref(json);
        free(write_buffer);
        return -1;
    }
    write_buffer[required_size] = 0;

    *out_buf = write_buffer;
    *out_size = required_size + 1;

    json_decref(json); // use this instead of free to prevent memory leaks

    return 0;
}

int main() {
    // \u0000
    {
        char template[] = "[\"\\u0000\"]";
        char** out_buf = NULL;
        size_t out_size;

        int ret = run(template, strlen(template), (char**)&(out_buf), &out_size);
        if (ret != 0) {
            printf("Failed to parse \\u0000\n");
        }
    }

    return 0;
}