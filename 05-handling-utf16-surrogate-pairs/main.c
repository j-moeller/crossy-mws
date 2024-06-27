#include "json-builder/json-builder.h"
#include "json-parser/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char buf[] = "{\"\\uDBFF\\uDFFF\": 0}";
    size_t size = sizeof(buf) / sizeof(*buf);

    json_settings settings = {};
    settings.value_extra = json_builder_extra; /* space for json-builder state */

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

    for (int i = 0; i < required_size; i++) {
        if (0x20 <= json_buf[i] && json_buf[i] < 0x7f) {
            printf("%d\n", json_buf[i]);
        } else {
            printf("0x%x\n", json_buf[i] & 0xff);
        }
    }

    free(json_buf);
    json_builder_free(json);

    return 0;
}
