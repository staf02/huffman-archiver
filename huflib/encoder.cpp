#include "encoder.h"
#include <iostream>
#include "buffered_writer.h"

encoder::encoder(std::istream& file_src) : src(file_src), freq(), tr() {
    if (!src.has_next()) {
        return;
    }
    count_freq();
    tr.build_by_freq(freq);
    tr.gen_codes();
}

void encoder::save_to_file(std::ostream& file_dst) {
    buffered_writer dst(file_dst);
    src.reset();
    if (!src.has_next()) {
        dst.write(255);
        return;
    }
    dst.write(tr.count_mod(freq));
    tr.print_to_file(dst);
    print_text(dst);
}

void encoder::count_freq() {
    unsigned char c;
    while (src.get_next(c)) {
        ++freq[c];
    }
}

void encoder::print_text(buffered_writer& dst) {
    unsigned char c;
    unsigned char buf = 0;
    const size_t BUFF_SIZE = 8;
    int16_t pos = 0;
    while (src.get_next(c)) {
        for (int i = 0; i < tr.codes[c].size() - 1; i++) {
            buf |= tr.codes[c][i] >> pos;
            dst.write(buf);
            buf = ((tr.codes[c][i]) << (BUFF_SIZE - pos));
        }
        buf |= tr.codes[c].back() >> (pos);
        int last_len = tr.lens[c] % BUFF_SIZE == 0 ? BUFF_SIZE : tr.lens[c] % BUFF_SIZE;
        if (pos + last_len >= BUFF_SIZE) {
            dst.write(buf);
            buf = tr.codes[c].back() << (BUFF_SIZE - pos);
        }
        pos = (pos + last_len) % BUFF_SIZE;
    }
    if (pos != 0) {
        dst.write(buf);
    }
}