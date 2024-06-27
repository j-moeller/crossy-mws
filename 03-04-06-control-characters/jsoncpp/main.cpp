#include <cstdlib>

#include "jsoncpp/include/json/json.h"
#include "jsoncpp/include/json/reader.h"

extern "C" {
int run(const char* buf, size_t size, char** out_buf, size_t* out_size);
}

int run(const char* buf, size_t size, char** out_buf, size_t* out_size)
{
    Json::Value json;
    Json::CharReaderBuilder char_reader_builder;
    std::unique_ptr<Json::CharReader> const char_reader(
        char_reader_builder.newCharReader());
    if (!char_reader->parse(buf, buf + size, &json, nullptr)) {
        return -1;
    }

    // use a specialized string builder to generate output without newlines and
    // indents
    Json::StreamWriterBuilder stream_writer_builder;
    stream_writer_builder["indentation"] = "";
    stream_writer_builder["emitUTF8"] = true;
    std::string s = Json::writeString(stream_writer_builder, json);

    const char* ss_buf = s.c_str();
    int ss_buf_len = strlen(ss_buf);

    // Use malloc() here, because we later call free() on the buffer
    char* ret_buf = static_cast<char*>(malloc(sizeof(char) * (ss_buf_len + 1)));
    if (ret_buf == nullptr) {
        return -1;
    }

    ret_buf[ss_buf_len] = 0;
    memcpy(ret_buf, ss_buf, ss_buf_len);

    *out_buf = ret_buf;
    *out_size = ss_buf_len + 1;

    return 0;
}


int main() {
    // 0x00 - 0x1f
    for (char i = 0x00; i <= 0x1f; i++) {
        char template_str[] = "[\"0\"]";
        int size = strlen(template_str);
        template_str[2] = i;

        char* out_buf = NULL;
        size_t out_size;

        int ret = run(template_str, size, (char**)&(out_buf), &out_size);

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