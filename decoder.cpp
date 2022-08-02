#include "decoder.h"
#include "buffered_writer.h"

decoder::decoder(const char* filename) : source(filename), tr() {};

void decoder::save_to_file(const char* filename) {
    unsigned char mod;
    source.get_next(mod);
    build_tree();   
    buffered_writer out(filename);
    decode_data(out, mod);
}

void decoder::build_tree() {
    tr.build_from_file(source);
    tr.gen_codes();
}

void decoder::decode_data(buffered_writer& out, uint8_t mod) {
    bool t;
    while (source.read_bit(t)) {
        tr.go_to(t);
        if (tr.is_code()) {
            out.write(tr.get_if_code());
        }
        if (source.bits_left() == mod) {
            return;
        }
        
    }
    return;
}
