#include "decoder.h"
#include "buffered_writer.h"

decoder::decoder(const char* filename) : source(filename), tr() {};

void decoder::save_to_file(const char* filename) {
    unsigned char mod;
    source.get_next(mod);
    tr.build_from_file(source);
    buffered_writer out(filename);
    decode_data(out, mod);
}

void decoder::decode_data(buffered_writer& out, uint8_t mod) {
    unsigned char c;
    while (source.get_next(c)) {
        int end_pos = 0;
        if (!source.has_next()) {
            end_pos = mod;
        }
        for (int i = 7; i >= end_pos; i--) {
          bool t = (c & (1 << i)) > 0;
          tr.go_to(t);
          if (tr.is_code()) {
            out.write(tr.get_if_code());
          }
        }
    }
    return;
}
