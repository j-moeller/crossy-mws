#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "frozen/frozen.h"

#define BUFFER_SIZE 8192

int run(const char* buf, size_t size, char** out_buf, size_t* out_size)
{
    char* json_buf = (char*)malloc(BUFFER_SIZE);
    if (NULL == json_buf) {
        return -1;
    }

    struct json_out out = JSON_OUT_BUF(json_buf, BUFFER_SIZE);
    if (0 > json_prettify(buf, size, &out)) {
        free(json_buf);
        return -1;
    }

    *out_buf = json_buf;
    *out_size = out.u.buf.len;

    return 0;
}

int main() {
    // Inserts commas
    {
        char template[] = "[1 2 3]";
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

        printf("%s\n=====\n", out_buf);
        free(out_buf);
    }

    // Inserts quotes
    {
        char template[] = "{abc: 0}";
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

        printf("%s\n=====\n", out_buf);
        free(out_buf);
    }

    // Inserts commas
    {
        char template[] = "[3[4]5true]";
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

        printf("%s\n=====\n", out_buf);
        free(out_buf);
    }

    // Removes empty keys
    {
        char template[] = "{\"\": 0}";
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

        printf("%s\n=====\n", out_buf);
        free(out_buf);
    }

    {
        char template[] = "{\"id\": 0,}";
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

        printf("%s\n=====\n", out_buf);
        free(out_buf);
    }

    // Stops parsing after full JSON
    {
        char template[] = "[\"x\"]]THIS IS IGNORED";
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

        printf("%s\n=====\n", out_buf);
        free(out_buf);
    }
}
