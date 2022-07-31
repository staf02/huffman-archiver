#include "decoder.h"
#include "buffered_writer.h"

decoder::decoder(const char* filename) : source(filename) {};

void decoder::save_to_file(const char* filename) {
    build_tree();   
    buffered_writer out(filename);
    decode_data(out);
}

void decoder::build_tree() {
    std::vector<bool> tmp;
    std::vector<int> stat(256);
    bool is_symbol = false;
    int16_t count_of_symbols;
    unsigned char c, d;
    source.get_next(c);
    if (c == 0) {
        count_of_symbols = 256;
    }
    else {
        count_of_symbols = c;
    }
    for (int16_t i = 0; i < count_of_symbols; i++) {
        source.get_next(c);
        source.get_next(d);
        stat[c] = d;
    }
    bool t;
    for (size_t i = 0; i < 256; i++) {
        tmp.clear();
        for (int j = 0; j < stat[i]; j++) {
            source.read_bit(t);
            tmp.push_back(t);
        }
        dict[tmp] = static_cast<unsigned char>(i);
    }
}

void decoder::decode_data(buffered_writer& out) {
    std::vector<bool> tmp;
    bool t;
    while (source.read_bit(t)) {
        tmp.push_back(t);
        if (dict.find(tmp) != dict.end()) {
            out.write(dict[tmp]);
            tmp.clear();
        }
    }
}
