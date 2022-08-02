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
    tr.gen_codes();
    out.write(count_mod());
    tr.print_to_file(out);
    auto x = clock();
    std::cout << clock() << std::endl;
    print_text(out);
    std::cout << clock() - x << std::endl;
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
        out.write_bit_array(tr.get_code(c));
    }
}

unsigned char encoder::count_not_nulls() {
    unsigned char count = 0;
    for (size_t i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            ++count;
        }
    }
    return count;
}

unsigned char encoder::count_mod() {
    uint64_t len = 0;
    for (size_t i = 0; i < 256; ++i) {
        len += (freq[i]) * tr.get_code(i).size();
    }
    return (8 - (len % 8)) % 8;
}
