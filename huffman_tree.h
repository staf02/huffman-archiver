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
    void gen_codes();
    std::vector<bool>& get_code(unsigned char c);
    void print_to_file(buffered_writer&);

    void build_from_file(buffered_reader&);

    void go_to(bool const& x);
    bool is_code();
    unsigned char get_if_code();

    size_t size();

private:
    class node {
    public:
        int l, r;
        node();
        node(int l, int r);
    };

    size_t const ALPHABET_SIZE = 256;

    std::vector<node> tree;
    std::string char_stor;
    std::vector<std::vector<bool>> codes;
    int actual_vertex, root;
    std::vector<bool> lists;

    void dfs(int v, std::vector<bool> &code);
};