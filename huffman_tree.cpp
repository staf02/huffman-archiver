#include "huffman_tree.h"
#include <queue>
#include <iostream>

huffman_tree::node::node() : l(-1), r(-1) {};

huffman_tree::node::node(int16_t l, int16_t r) : l(l), r(r) {};

huffman_tree::huffman_tree() : actual_vertex(-1), root(-1), tree(), p() {
}

void huffman_tree::build_by_freq(std::array<uint64_t, 256> const& arr) {
    std::priority_queue<std::pair<uint64_t, int16_t>, 
        std::vector<std::pair<uint64_t, int16_t>>, 
        std::greater<std::pair<uint64_t, int16_t>>> q;
    int16_t cnt = 0, last_index = ALPHABET_SIZE;
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        if (arr[i] != 0) {
            q.emplace(arr[i], i);
            cnt++;
            
        }
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
    if (tree.size() == ALPHABET_SIZE) {
        tree.push_back(node(q.top().second, -1));
        p[q.top().second] = ALPHABET_SIZE;
    }
}

void huffman_tree::print_code(buffered_writer& out, unsigned char const& c) {
    go_up(out, c, p[c]);
}

void huffman_tree::print_to_file(buffered_writer& dist) {
    if (tree.back().r == -1) {
        dist.write(bool(0));
        dist.write((unsigned char) tree.back().l);
        return;
    }
    unsigned char nodes_count = tree.size() - ALPHABET_SIZE;
    dist.write(nodes_count);
    for (size_t i = ALPHABET_SIZE; i < tree.size(); ++i) {
        unsigned char l1 = tree[i].l / ALPHABET_SIZE, l2 = tree[i].l % ALPHABET_SIZE, r1 = tree[i].r / ALPHABET_SIZE, r2 = tree[i].r % ALPHABET_SIZE;
        unsigned char c = l1 + r1 * 2;
        dist.write(static_cast<unsigned char>(tree[i].l / ALPHABET_SIZE + tree[i].r / ALPHABET_SIZE * 2));
        dist.write(static_cast<unsigned char>(tree[i].l % ALPHABET_SIZE));
        dist.write(static_cast<unsigned char>(tree[i].r % ALPHABET_SIZE));
    }
}

unsigned char huffman_tree::count_mod(std::array<uint64_t, 256> const& arr) {
    std::vector<int16_t> len(ALPHABET_SIZE, 0);
    root = actual_vertex = tree.size() - 1;
    count_len(len, root);
    int16_t mod = 0;
    for (int16_t i = 0; i < ALPHABET_SIZE; i++) {
        mod += arr[i] * len[i];
    }
    mod = (8 - (mod % 8)) % 8;
    return mod;
}

void huffman_tree::build_from_file(buffered_reader& source) {
    unsigned char nodes_count;
    source.get_next(nodes_count);
    tree.resize(ALPHABET_SIZE + nodes_count);
    if (nodes_count == 0) {
       unsigned char c;
       source.get_next(c);
       tree.push_back(node(c, -1));
    }
    else {
        int16_t last_index = ALPHABET_SIZE;
        for (size_t i = 0; i < nodes_count; i++) {
            unsigned char c;
            unsigned char u, v;
            source.get_next(c);
            source.get_next(u);
            source.get_next(v);
            int16_t x = u, y = v;
            if (c & 1) {
                x += ALPHABET_SIZE;
            }
            if (c & 2) {
                y += ALPHABET_SIZE;
            }
            tree[last_index++] = node(x, y);
        }
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

void huffman_tree::go_up(buffered_writer& out, int16_t v, int16_t u) {
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

void huffman_tree::count_len(std::vector<int16_t> &len, int16_t v, int16_t h) {
    if (v == -1) {
        return;
    }
    if (v < ALPHABET_SIZE) {
        len[v] = h;
    }
    count_len(len, tree[v].l, h + 1);
    count_len(len, tree[v].r, h + 1);
}
