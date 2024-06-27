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
    // Removes string on first element
    {
        char template[] = "[\"THIS IS REMOVED\tAND THIS TOO\"]";
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

    {
        char template[] = "{\"THIS IS REMOVED\tAND THIS TOO\": 10}";
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

    {
        char template[] = "[\"x\"]THIS IS IGNORED";
        int size = strlen(template);

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

    return 0;
}