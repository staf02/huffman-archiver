#pragma once
#include "huffman_tree.h"
#include <unordered_map>
#include "buffered_reader.h"
#include "buffered_writer.h"

class decoder {
public:
    decoder(const char*);
    decoder() = delete;
    decoder(decoder& other) = delete;
    decoder& operator=(decoder& r) = delete;
    ~decoder() = default;

    void save_to_file(const char*);

private:
    buffered_reader src;
    huffman_tree tr;

    void decode_data(buffered_writer &out, uint8_t mod);
};