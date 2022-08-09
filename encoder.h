#pragma once
#include "buffered_reader.h"
#include "fstream"
#include "huffman_tree.h"
#include <array>
#include "buffered_writer.h"

class encoder {
public:
    encoder() = delete;
    encoder(const char*);
    encoder(encoder& other) = delete;
    encoder& operator=(encoder& r) = delete;
    ~encoder() = default;
    void save_to_file(const char*);

private:
    buffered_reader source;
    std::array<uint64_t, 256> freq;
    huffman_tree tr;

    void count_freq();
    void print_text(buffered_writer &out);
};