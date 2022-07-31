#include "decoder.h"
#include "buffered_writer.h"

decoder::decoder(const char* filename) : source(filename) {};

void decoder::save_to_file(const char* filename) {
    unsigned char mod;
    source.get_next(mod);
    build_tree();   
    buffered_writer out(filename);
    decode_data(out, mod);
}

std::vector<bool> decoder::splice(std::vector<bool> const& src, std::vector<bool>::iterator const& begin, std::vector<bool>::iterator const& end) {
    std::vector<bool> result;
    for (auto it = begin; it != end; it++) {
        result.push_back(*it);
    }
    return result;
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

void decoder::decode_data(buffered_writer& out, uint8_t mod) {
    std::vector<bool> tmp;
    bool t;
    while (source.read_bit(t)) {
        tmp.push_back(t);
        if (tmp.size() > mod) {
            size_t end_pos = tmp.size() - mod;
            auto tmp1 = splice(tmp, tmp.begin(), tmp.begin() + end_pos);
            if (dict.find(tmp1) != dict.end()) {
                out.write(dict[tmp1]);
                tmp.erase(tmp.begin(), tmp.begin() + end_pos);
            }
        }
    }
    return;
}
