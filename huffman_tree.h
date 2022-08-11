#pragma once
#include <vector>
#include "buffered_writer.h"
#include "buffered_reader.h"
#include <array>
#include <unordered_map>

class huffman_tree {
public:
    std::vector<std::vector<unsigned char>> codes;
    std::vector<uint16_t> lens;
    huffman_tree();
    huffman_tree(huffman_tree& other) = delete;
    huffman_tree& operator=(huffman_tree& r) = delete;
    ~huffman_tree() = default;

    void build_by_freq(std::array<uint64_t, 256> const&);
    unsigned char count_mod(std::array<uint64_t, 256> const&);
    void print_to_file(buffered_writer&);
    void gen_codes();
    void build_from_file(buffered_reader&);

    void go_to(bool const& x);
    bool is_code();
    unsigned char get_if_code();
    void go_to_c(buffered_writer &out, unsigned char c);


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
    std::vector<std::vector<int>> dp;
    std::vector<std::vector<std::string>> str_dp;
    std::string one_symbol_string = "";

    void dfs(int16_t v, std::vector<unsigned char> &code_arr, unsigned char c = 0, int16_t len = 0);
    void count_dp(int16_t v, int16_t u);
};