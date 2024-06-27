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

    {
        char template[] = "[\"\\uD800\"]";
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