#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "json-c/json_object.h"
#include "json-c/json_tokener.h"

int run(const char* buf, size_t size, char** out_buf, size_t* out_size)
{
    struct json_tokener* tok = json_tokener_new();

    // enum json_tokener_error jerr;
    json_object* json;

    json = json_tokener_parse_ex(tok, buf, size);
    if (json == NULL || json_tokener_get_error(tok) != json_tokener_success) {
        json_tokener_free(tok);
        json_object_put(json);
        return -1;
    }

    size_t length;
    const char* json_string = json_object_to_json_string_length(
        json, JSON_C_TO_STRING_PLAIN, &length);
    if (json_string == NULL) {
        json_tokener_free(tok);
        json_object_put(json);
        return -1;
    }

    char* new_buf = malloc(length + 1);
    if (new_buf == NULL) {
        json_tokener_free(tok);
        json_object_put(json);
        return -1;
    }

    memcpy(new_buf, json_string, length);
    new_buf[length] = 0;
    *out_size = length + 1;
    *out_buf = new_buf;

    json_tokener_free(tok);
    json_object_put(json);
    return 0;
}

int main() {
    // 0x01 - 0x1f
    for (char i = 0x01; i <= 0x1f; i++) {
        char template[] = "[\"0\"]";
        int size = strlen(template);
        template[2] = i;

        char* out_buf = NULL;
        size_t out_size;

        int ret = run(template, size, (char**)&(out_buf), &out_size);

        if (ret != 0) {
            exit(1);
        }

        if (out_size == 0 || out_buf == NULL) {
            exit(1);
        }

        printf("%s\n", out_buf);
        free(out_buf);
    }
}