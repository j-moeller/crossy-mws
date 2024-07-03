#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ccan/ccan/json/json.h"

int run(const char* buf, size_t size, char** out_buf, size_t* out_size)
{
    // The ccan json lib only accepts nullterminated strings
    char* terminated_buffer = malloc(sizeof(char) * (size + 1));
    if (terminated_buffer == NULL) {
        return -1;
    }
    memcpy(terminated_buffer, buf, size);
    terminated_buffer[size] = 0;

    JsonNode* json = json_decode(terminated_buffer);

    if (json == NULL) {
        free(terminated_buffer);
        return -1;
    }

    char* json_buf = json_encode(json);
    int json_buf_len = strlen(json_buf);

    char* ret_buf = malloc(sizeof(char) * (json_buf_len + 1));
    if (ret_buf == NULL) {
        free(json_buf);
        free(terminated_buffer);
        json_delete(json);
        return -1;
    }
    ret_buf[json_buf_len] = 0;
    memcpy(ret_buf, json_buf, json_buf_len);

    *out_buf = ret_buf;
    *out_size = json_buf_len + 1;

    free(json_buf);
    free(terminated_buffer);
    json_delete(json);

    return 0;
}

int main() {
    {
        char template[] = "[\"\\u001f\"]";
        char* out_buf = NULL;
        size_t out_size;

        int ret = run(template, strlen(template), (char**)&(out_buf), &out_size);

        if (ret != 0) {
            exit(1);
        }

        if (out_buf == NULL) {
            exit(1);
        }

        printf("%s\n", out_buf);
        free(out_buf);
    }

    // \u0000
    {
        char template[] = "[\"\\u0000\"]";
        char* out_buf = NULL;
        size_t out_size;

        int ret = run(template, strlen(template), (char**)&(out_buf), &out_size);
        if (ret != 0) {
            printf("Failed to parse \\u0000\n");
        }
    }

    // 0x1f
    {
        char template[] = "[\"0\"]";
        int size = strlen(template);
        template[2] = 0x1f;

        char* out_buf = NULL;
        size_t out_size;

        int ret = run(template, size, (char**)&(out_buf), &out_size);
        if (ret != 0) {
            printf("Failed to parse 0x1f\n");
        }
    }

    return 0;
}
