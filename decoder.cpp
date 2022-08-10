#include "decoder.h"
#include "buffered_writer.h"

decoder::decoder(const char* filename) : src(filename), tr() {};

void decoder::save_to_file(const char* filename) {
    buffered_writer dst(filename);
    if (!src.has_next()) {
        return;
    }
    unsigned char mod;
    src.get_next(mod);
    tr.build_from_file(src);
    decode_data(dst, mod);
}

void decoder::decode_data(buffered_writer& dst, uint8_t mod) {
    unsigned char c;
    while (src.get_next(c)) {
        int end_pos = 0;
        if (!src.has_next()) {
            end_pos = mod;
        }
        if (end_pos != 0) {
            for (int i = 7; i >= end_pos; i--) {
              bool t = (c & (1 << i)) > 0;
              tr.go_to(t);
              if (tr.is_code()) {
                dst.write(tr.get_if_code());
              }
            }
        }
        else {
            tr.go_to_c(dst, c);
        }
    }
    return;
}
