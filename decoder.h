#pragma once
#include "huffman_tree.h"
#include <unordered_map>
#include "buffered_reader.h"
#include "buffered_writer.h"

class decoder {
public:
    decoder(const char*);
    void save_to_file(const char*);

private:
    buffered_reader source;
    std::unordered_map<std::vector<bool>, unsigned char> dict;
    huffman_tree tr;

    void build_tree();
    void decode_data(buffered_writer &out, uint8_t mod);
};