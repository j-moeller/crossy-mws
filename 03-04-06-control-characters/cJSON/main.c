#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cJSON/cJSON.h"

int run(const char* buf, size_t size, char** out_buf, size_t* out_size)
{
    cJSON* json = cJSON_ParseWithLength(buf, size);
    if (json == NULL) {
        return -1;
    }

    char* json_buf = cJSON_PrintUnformatted(json);
    int json_buf_len = strlen(json_buf);

    char* ret_buf = malloc(sizeof(char) * (json_buf_len + 1));
    if (ret_buf == NULL) {
        free(json_buf);
        cJSON_Delete(json);
        return -1;
    }
    ret_buf[json_buf_len] = 0;
    memcpy(ret_buf, json_buf, json_buf_len);

    *out_buf = ret_buf;
    *out_size = json_buf_len + 1;

    free(json_buf);
    cJSON_Delete(json);

    return 0;
}

int main() {
    // 0x00
    {
        char template[] = "[\"0Truncated\"]";
        int size = strlen(template);
        template[2] = 0;

        char* out_buf = NULL;
        size_t out_size;

        int ret = run(template, size, (char**)&(out_buf), &out_size);

        if (ret != 0) {
            exit(1);
        }

        if (out_buf == NULL) {
            exit(1);
        }

        printf("%s\n", out_buf);
        free(out_buf);
    }

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
        char template[] = "[\"\\u0000Truncated\"]";
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

    return 0;
}