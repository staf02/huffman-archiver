#include "huffman_tree.h"
#include <queue>
#include <iostream>

huffman_tree::node::node() : l(-1), r(-1) {};

huffman_tree::node::node(int l, int r) : l(l), r(r) {};


huffman_tree::huffman_tree() : lists(), char_stor(""), actual_vertex(-1), root(-1) {
    tree = std::vector<node>();
    codes.resize(ALPHABET_SIZE);
}

void huffman_tree::build_by_freq(std::array<uint64_t, 256> const& arr) {
    std::priority_queue<std::pair<uint64_t, int>, 
        std::vector<std::pair<uint64_t, int>>, 
        std::greater<std::pair<uint64_t, int>>> q;
    for (size_t i = 0; i < 256; ++i) {
        if (arr[i] != 0) {
            char_stor += static_cast<unsigned char>(i);
            q.emplace(arr[i], tree.size());
            tree.push_back(node());
        }
    }
    while (q.size() != 1) {
        auto u = q.top();
        q.pop();
        auto v = q.top();
        q.pop();
        q.emplace(u.first + v.first, tree.size());
        tree.push_back(node(u.second, v.second));
    }
    if (tree.size() == 1) {
        tree.push_back(node(0, -1));
    }
    root = actual_vertex = tree.size() - 1;
}

void huffman_tree::gen_codes() {
    std::vector<bool> code;
    dfs(tree.size() - 1, code);
}

std::vector<bool>& huffman_tree::get_code(unsigned char c) {
    return codes[c];
}

void huffman_tree::print_to_file(buffered_writer& dist) {
    unsigned char lists_count = char_stor.size(), nodes_count = tree.size() - char_stor.size();
    dist.write(lists_count);
    if (lists_count == 1) {
        dist.write(char_stor);
        return;
    }
    dist.write(nodes_count);
    dist.write(char_stor);
    for (size_t i = char_stor.length(); i < tree.size(); ++i) {
        unsigned char l1 = tree[i].l / 256, l2 = tree[i].l % 256, r1 = tree[i].r / 256, r2 = tree[i].r % 256;
        dist.write(l1);
        dist.write(l2);
        dist.write(r1);
        dist.write(r2);
    }
}

void huffman_tree::build_from_file(buffered_reader& source) {
    unsigned char list_count;
    source.get_next(list_count);
    if (list_count == 1) {
        unsigned char c;
        source.get_next(c);
        char_stor += c;
        tree.push_back(node());
        tree.push_back(node(0, -1));
        actual_vertex = root = 1;
        return;
    }
    unsigned char nodes_count;
    source.get_next(nodes_count);
    int t = (list_count - 1 + 256) % 256 + 1;
    for (size_t i = 0; i < t; i++) {
        unsigned char c;
        source.get_next(c);
        char_stor += c;
        tree.push_back(node());
    }
    for (size_t i = 0; i < nodes_count; i++) {
        unsigned char u, v;
        source.get_next(u);
        source.get_next(v);
        int x = u * 256 + v;
        source.get_next(u);
        source.get_next(v);
        int y = u * 256 + v;
        tree.push_back(node(x, y));
    }
    lists.resize(tree.size());
    for (size_t i = 0; i < char_stor.length(); i++) {
        lists[i] = true;
    }
    root = actual_vertex = tree.size() - 1;
    return;
}

void huffman_tree::go_to(bool const& x) {
    if (x == false) {
        actual_vertex = tree[actual_vertex].l;
    }
    else {
        actual_vertex = tree[actual_vertex].r;
    }
}

bool huffman_tree::is_code() {
    return actual_vertex < char_stor.size();
}

unsigned char huffman_tree::get_if_code() {
    unsigned char c = char_stor[actual_vertex];
    actual_vertex = tree.size() - 1;
    return c;
}


size_t huffman_tree::size() {
    return tree.size();
}

void huffman_tree::dfs(int v, std::vector<bool>& code) {
    if (v < char_stor.size()) {
        codes[static_cast<unsigned char>(char_stor[v])] = code;
        return;
    }
    code.push_back(0);
    dfs(tree[v].l, code);
    code.pop_back();
    if (tree[v].r != -1) {
        code.push_back(1);
        dfs(tree[v].r, code);
        code.pop_back();
    }
}
