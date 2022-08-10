#include "encoder.h"
#include <queue>
#include <utility>
#include <memory>
#include <iostream>
#include "buffered_writer.h"

encoder::encoder(const char* filename) : source(filename), freq(), tr() {   
    count_freq();
    tr.build_by_freq(freq);
    tr.gen_codes();
}

void encoder::save_to_file(const char* filename) {
    buffered_writer out(filename);
    out.write(tr.count_mod(freq));
    tr.print_to_file(out);
    print_text(out);
    out.close();
}

void encoder::count_freq() {
    unsigned char c;
    while (source.get_next(c)) {
        ++freq[c];
    }
}

void encoder::print_text(buffered_writer& out) {
    source.reset();
    unsigned char c;
    unsigned char buf = 0;
    int16_t pos = 0, pos_in_arr = 0, bit_pos = 0;
    while (source.get_next(c)) {
        for (int i = 0; i < tr.codes[c].size() - 1; i++) {
            buf += tr.codes[c][i] >> pos;
            out.write(buf);
            buf = ((tr.codes[c][i]) << (8 - pos));
        }
        buf |= tr.codes[c].back() >> (pos);
        int last_len = tr.lens[c] % 8 == 0 ? 8 : tr.lens[c] % 8;
        if (pos + last_len >= 8) {
            out.write(buf);
            buf = tr.codes[c].back() << (8 - pos);
            pos = (pos + last_len) % 8;
        }
        else {
            pos += last_len;
        }
    }
    if (pos != 0) {
        out.write(buf);
    }
}