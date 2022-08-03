#include "huffman_tree.h"
#include <queue>
#include <iostream>

huffman_tree::node::node() : l(-1), r(-1) {};

huffman_tree::node::node(int l, int r) : l(l), r(r) {};

huffman_tree::huffman_tree() : lists(), char_stor(""), actual_vertex(-1), root(-1), mod(0) {
    tree = std::vector<node>();
}

void huffman_tree::build_by_freq(std::array<uint64_t, 256> const& arr) {
    std::priority_queue<std::pair<uint64_t, int>, 
        std::vector<std::pair<uint64_t, int>>, 
        std::greater<std::pair<uint64_t, int>>> q;
    int cnt = 0, last_index = ALPHABET_SIZE;
    unsigned char c;
    for (size_t i = 0; i < 256; ++i) {
        if (arr[i] != 0) {
            c = i;
            q.emplace(arr[i], i);
            cnt++;
            
        }
    }
    if (cnt == 1) {
        char_stor += c;
        return;
    }
    tree.resize(ALPHABET_SIZE + cnt - 1);
    p.resize(ALPHABET_SIZE + cnt - 1);
    while (q.size() != 1) {
        auto u = q.top();
        q.pop();
        auto v = q.top();
        q.pop();
        q.emplace(u.first + v.first, last_index);
        tree[last_index] = node(u.second, v.second);
        p[u.second] = p[v.second] = last_index;
        last_index++;
    }
    std::vector<int> len(ALPHABET_SIZE, 0);
    root = actual_vertex = tree.size() - 1;
    count_mod(len, root);
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        mod += arr[i] * len[i];
    }
    mod = (8 - (mod % 8)) % 8;
}

void huffman_tree::print_code(buffered_writer& out, unsigned char const& c) {
    go_up(out, c, p[c]);
}

void huffman_tree::print_to_file(buffered_writer& dist) {
    dist.write((unsigned char) mod);
    if (tree.size() == 0) {
        dist.write(1);
        dist.write(char_stor);
        return;
    }
    unsigned char nodes_count = tree.size() - ALPHABET_SIZE;
    dist.write(nodes_count);
    for (size_t i = ALPHABET_SIZE; i < tree.size(); ++i) {
        unsigned char l1 = tree[i].l / 256, l2 = tree[i].l % 256, r1 = tree[i].r / 256, r2 = tree[i].r % 256;
        dist.write(l1);
        dist.write(l2);
        dist.write(r1);
        dist.write(r2);
    }
}

void huffman_tree::build_from_file(buffered_reader& source) {
    /*unsigned char list_count;
    source.get_next(list_count);
    if (list_count == 1) {
        unsigned char c;
        source.get_next(c);
        char_stor += c;
        tree.push_back(node());
        tree.push_back(node(0, -1));
        actual_vertex = root = 1;
        return;
    }*/
    unsigned char nodes_count;
    source.get_next(nodes_count);
    tree.resize(ALPHABET_SIZE + nodes_count);
    int last_index = ALPHABET_SIZE;
    for (size_t i = 0; i < nodes_count; i++) {
        unsigned char u, v;
        source.get_next(u);
        source.get_next(v);
        int x = u * 256 + v;
        source.get_next(u);
        source.get_next(v);
        int y = u * 256 + v;
        tree[last_index++] = node(x, y);
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
    return actual_vertex < ALPHABET_SIZE;
}

unsigned char huffman_tree::get_if_code() {
    unsigned char c = actual_vertex;
    actual_vertex = tree.size() - 1;
    return c;
}

void huffman_tree::go_up(buffered_writer& out, int v, int u) {
    if (v != tree.size() - 1) {
        go_up(out, p[v], v);
    }
    if (u == tree[v].l) {
        out.write_bit((bool) 0);
    }
    else if (u == tree[v].r) {
        out.write_bit(1);
    }
}


size_t huffman_tree::size() {
    return tree.size();
}

void huffman_tree::count_mod(std::vector<int> &len, int v, int h) {
    if (v == -1) {
        return;
    }
    if (v < ALPHABET_SIZE) {
        len[v] = h;
    }
    count_mod(len, tree[v].l, h + 1);
    count_mod(len, tree[v].r, h + 1);
}
