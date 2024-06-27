#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h/json.h"

int run(const char* buf, size_t size, char** out_buf, size_t* out_size)
{
    struct json_value_s* root = json_parse(buf, size);
    char* json_buf = json_write_minified(root, out_size);
    if (json_buf == NULL) {
        if (root != NULL) {
            free(root);
        }
        return -1;
    }

    int json_buf_len = strlen(json_buf);

    char* ret_buf = malloc(sizeof(char) * (json_buf_len + 1));
    ret_buf[json_buf_len] = 0;
    memcpy(ret_buf, json_buf, json_buf_len);

    *out_buf = ret_buf;
    *out_size = json_buf_len + 1;

    free(json_buf);
    free(root);

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

    // \u0000
    {
        char template[] = "[\"\\u0000Invalid\"]";
        int size = strlen(template);

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

    // \u0001 - \u0007
    for (int i = 1; i <= 7; i++) {
        char template[] = "[\"\\u0000Invalid\"]";
        int size = strlen(template);

        template[7] = '0' + i;

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