#pragma once
#include "huffman_tree.h"
#include <unordered_map>
#include "buffered_reader.h"

class decoder {
public:
    decoder(const char*);
    void save_to_file(const char*);

private:
    buffered_reader source;
    std::unordered_map<std::string, unsigned char> dict;

    void build_tree();
    void decode_data(std::ofstream &out);
};