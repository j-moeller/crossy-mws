#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json-builder/json-builder.h"
#include "json-parser/json.h"

int run(const char* buf, size_t size, char** out_buf, size_t* out_size)
{
    json_settings settings = {};
    settings.value_extra =
        json_builder_extra; /* space for json-builder state */

    char error[128];
    json_value* json = json_parse_ex(&settings, buf, size, error);
    if (json == NULL) {
        return -1;
    }

    json_serialize_opts serialize_opts = {};
    serialize_opts.mode = json_serialize_mode_packed;
    serialize_opts.opts = json_serialize_opt_pack_brackets |
                          json_serialize_opt_no_space_after_comma |
                          json_serialize_opt_no_space_after_colon;

    size_t required_size = json_measure_ex(json, serialize_opts);

    char* json_buf;
    if (NULL == (json_buf = malloc(required_size + 1))) {
        return -1;
    }

    json_serialize_ex(json_buf, json, serialize_opts);
    json_buf[required_size] = 0;

    *out_buf = json_buf;
    *out_size = required_size + 1;
    json_builder_free(json);

    return 0;
}

int main() {
    // 0x01 - 0x1f
    for (char i = 0x01; i < 0x1f; i++) {
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

        printf("0x%x: %s\n", i, out_buf);
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

        printf("\\u0000: %s\n", out_buf);
        free(out_buf);
    }

    // \u0001 - \u0007
    for (int i = 1; i <= 7; i++) {
        char template[] = "[\"\\u0000\"]";
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

        printf("\\u000%d: %s\n", i, out_buf);
        free(out_buf);
    }

    // \u000b
    {
        char template[] = "[\"\\u000b\"]";
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

        printf("\\u000b: %s\n", out_buf);
        free(out_buf);
    }

    // \u000e - \u001f
    {
        const char *templates[] = {
            "[\"\\u000e\"]",
            "[\"\\u000f\"]",
            "[\"\\u0010\"]",
            "[\"\\u0011\"]",
            "[\"\\u0012\"]",
            "[\"\\u0013\"]",
            "[\"\\u0014\"]",
            "[\"\\u0015\"]",
            "[\"\\u0016\"]",
            "[\"\\u0017\"]",
            "[\"\\u0018\"]",
            "[\"\\u0019\"]",
            "[\"\\u001a\"]",
            "[\"\\u001b\"]",
            "[\"\\u001c\"]",
            "[\"\\u001d\"]",
            "[\"\\u001e\"]",
            "[\"\\u001f\"]"
        };

        for (int i = 0; i < sizeof(templates) / sizeof(*templates); i++) {
            char* template = templates[i];

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

            if (i < 2) {
                printf("\\u000%x: %s\n", i + 0xe, out_buf);
            } else {
                printf("\\u00%x: %s\n", i + 0xe, out_buf);
            }
            free(out_buf);
        }
    }
}