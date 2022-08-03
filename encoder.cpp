#include "encoder.h"
#include <queue>
#include <utility>
#include <memory>
#include <iostream>
#include "buffered_writer.h"

encoder::encoder() : source(), freq(), tr() {}

encoder::encoder(const char* filename) : source(filename), freq(), tr() {}

void encoder::open(const char* filename) {
    source.open(filename);
}

void encoder::encode() {
    count_freq();
    tr.build_by_freq(freq);
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
    while (source.get_next(c)) {
        tr.print_code(out, c);
    }
}