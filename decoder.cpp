#include "decoder.h"

decoder::decoder(const char* filename) : source(filename) {};

void decoder::save_to_file(const char* filename) {
    build_tree();   
    std::ofstream out(filename, std::ios::binary);
    decode_data(out);
    out.close();
}

void decoder::build_tree() {
    std::string tmp = "";
    bool is_symbol = false;
    while (!source.eof()) {
        unsigned char c = source.get_next();
        if (c == '\n' && tmp.empty()) {
            break;
        }
        if (c == ' ' && !is_symbol) {
            is_symbol = true;
        }
        else if (is_symbol) {
            source.get_next();
            dict[tmp] = c;
            tmp = "";
            is_symbol = false;
        }
        else {
            tmp += c;
        }
    }
}

void decoder::decode_data(std::ofstream& out) {
    std::string tmp = "";
    while (!source.eof()) {
        tmp += source.get_next();
        if (dict.find(tmp) != dict.end()) {
            out << dict[tmp];
            tmp = "";
        }
    }
}
