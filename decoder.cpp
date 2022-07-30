#include "decoder.h"
#include "buffered_writer.h"

decoder::decoder(const char* filename) : source(filename) {};

void decoder::save_to_file(const char* filename) {
    build_tree();   
    buffered_writer out(filename);
    decode_data(out);
    //out.close();
}

void decoder::build_tree() {
    std::string tmp = "";
    bool is_symbol = false;
    unsigned char c;
    while (source.get_next(c)) {
        if (c == '\n' && tmp.empty()) {
            break;
        }
        if (c == ' ' && !is_symbol) {
            is_symbol = true;
        }
        else if (is_symbol) {
            unsigned char t;
            source.get_next(t);
            dict[tmp] = c;
            tmp = "";
            is_symbol = false;
        }
        else {
            tmp += c;
        }
    }
}

void decoder::decode_data(buffered_writer& out) {
    std::string tmp = "";
    unsigned char c;
    while (source.get_next(c)) {
        tmp += c;
        if (dict.find(tmp) != dict.end()) {
            out.write(dict[tmp]);
            tmp = "";
        }
    }
}
