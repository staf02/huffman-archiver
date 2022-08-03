#pragma once
#include <vector>
#include "buffered_writer.h"
#include "buffered_reader.h"
#include <array>
#include <unordered_map>

class huffman_tree {
public:
    huffman_tree();
    void build_by_freq(std::array<uint64_t, 256> const&);
    unsigned char count_mod(std::array<uint64_t, 256> const&);
    void print_to_file(buffered_writer&);
    void print_code(buffered_writer&, unsigned char const&);
    void build_from_file(buffered_reader&);

    void go_to(bool const& x);
    bool is_code();
    unsigned char get_if_code();


private:
    class node {
    public:
        int16_t l, r;
        node();
        node(int16_t l, int16_t r);
    };

    size_t const ALPHABET_SIZE = 256;
    int16_t actual_vertex, root;
    std::vector<node> tree;
    std::vector<int16_t> p;

    void count_len(std::vector<int16_t> &len, int16_t v, int16_t h = 0);
    void go_up(buffered_writer&, int16_t v, int16_t u);
};