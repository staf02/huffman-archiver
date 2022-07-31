#pragma once
#include "buffered_reader.h"
#include "fstream"
#include "huffman_tree.h"
#include <array>
#include "buffered_writer.h"

class encoder {
public:
    encoder();
    encoder(const char*);
    ~encoder() = default;
    void open(const char*);
    void encode();
    void save_to_file(const char*);

private:
    node* tree_root;
    buffered_reader source;
    std::array<uint64_t, 256> freq;

    void count_freq();
    void build_tree();
    void gen_code(node* t, std::vector<std::vector<bool>> &, std::vector<bool>&);
    void print_codes(buffered_writer &out, std::vector<std::vector<bool>>&);
    void print_text(buffered_writer &out, std::vector<std::vector<bool>>&);
    unsigned char count_not_nulls();
};