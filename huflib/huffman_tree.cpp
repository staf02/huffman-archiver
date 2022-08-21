#include "huffman_tree.h"
#include <queue>
#include <iostream>
#include <algorithm>

huffman_tree::node::node() : l(-1), r(-1) {}

huffman_tree::node::node(int16_t l, int16_t r) : l(l), r(r) {}

huffman_tree::huffman_tree() : actual_vertex(-1), root(-1), tree() {
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
    while (q.size() != 1) {
        auto u = q.top();
        q.pop();
        auto v = q.top();
        q.pop();
        q.emplace(u.first + v.first, last_index);
        tree[last_index] = node(u.second, v.second);
        last_index++;
    }
    if (tree.size() == ALPHABET_SIZE) {
        tree.push_back(node(q.top().second, -1));
    }
}

void huffman_tree::print_to_file(buffered_writer& dst) {
    if (tree.back().r == -1) {
        dst.write(bool(0));
        dst.write((unsigned char) tree.back().l);
        return;
    }
    unsigned char nodes_count = tree.size() - ALPHABET_SIZE;
    dst.write(nodes_count);
    for (size_t i = ALPHABET_SIZE; i < tree.size(); ++i) {
        dst.write(static_cast<unsigned char>(tree[i].l / ALPHABET_SIZE + tree[i].r / ALPHABET_SIZE * 2));
        dst.write(static_cast<unsigned char>(tree[i].l % ALPHABET_SIZE));
        dst.write(static_cast<unsigned char>(tree[i].r % ALPHABET_SIZE));
    }
}

void huffman_tree::gen_codes() {
    codes.resize(ALPHABET_SIZE);
    lens.resize(ALPHABET_SIZE);
    std::vector<unsigned char> v;
    dfs(tree.size() - 1, v, 0, 0);
}

unsigned char huffman_tree::count_mod(std::array<uint64_t, 256> const& arr) {
    root = actual_vertex = tree.size() - 1;
    int16_t mod = 0;
    for (int16_t i = 0; i < ALPHABET_SIZE; i++) {
        mod += arr[i] * lens[i];
    }
    mod = (BUFF_SIZE - (mod % BUFF_SIZE)) % BUFF_SIZE;
    return mod;
}

void huffman_tree::build_from_file(buffered_reader& src) {
    unsigned char nodes_count;
    if (!src.get_next(nodes_count)) {
        throw std::runtime_error("empty huffman tree");
    }
    tree.resize(ALPHABET_SIZE + nodes_count);
    if (nodes_count == 0) {
       unsigned char c;
       src.get_next(c);
       tree.push_back(node(c, -1));
       root = actual_vertex = tree.size() - 1;
       for (size_t i = 0; i < BUFF_SIZE; i++) {
           one_symbol_string += c;
       }
    }
    else {
        std::vector<int> used(tree.size(), false);
        int16_t last_index = ALPHABET_SIZE;
        for (size_t i = 0; i < nodes_count; i++) {
            unsigned char c;
            unsigned char u, v;
            if (!src.get_next(c) || !src.get_next(u) || !src.get_next(v)) {
                throw std::runtime_error("incomplete huffman tree");
            }
            int16_t x = u, y = v;
            if (c & 1) {
                x += ALPHABET_SIZE;
            }
            if (c & 2) {
                y += ALPHABET_SIZE;
            }
            used[last_index] = 1;
            if (x >= last_index || y >= last_index || used[x] == 2 || used[y] == 2) {
                throw std::runtime_error("wrong huffman tree");
            }
            used[x] = 2;
            used[y] = 2;
            tree[last_index++] = node(x, y);
        }
        for (size_t i = 0; i < used.size() - 1; i++) {
            if (used[i] == 1) {
                throw std::runtime_error("wrong huffman tree");
            }
        }
        root = actual_vertex = tree.size() - 1;
        dp.resize(tree.size());
        str_dp.resize(tree.size());
        count_dp(root, -1);
        actual_vertex = root;
    }
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

void huffman_tree::go_to_c(buffered_writer& dst, unsigned char c) {
    if (tree.size() == 257) {
        dst.write(one_symbol_string);
    }
    else {
        dst.write(str_dp[actual_vertex][c]);
        actual_vertex = dp[actual_vertex][c];
    }
}

void huffman_tree::dfs(int16_t v, std::vector<unsigned char> &code_arr, unsigned char code, int16_t len) {
    if (v == -1) {
        return;
    }
    if (len % BUFF_SIZE == 0 && len != 0) {
        code_arr.push_back(code);
        code = 0;
    }
    if (v < ALPHABET_SIZE) {
        if (len % BUFF_SIZE != 0) {
            code_arr.push_back(code);
        }
        lens[v] = len;
        codes[v] = code_arr;
        if (len % BUFF_SIZE != 0) {
            code_arr.pop_back();
        }
    }
    dfs(tree[v].l, code_arr, code, len + 1);
    code += 1 << (BUFF_SIZE - 1 - (len % BUFF_SIZE));
    dfs(tree[v].r, code_arr, code, len + 1); 
    if (len % BUFF_SIZE == 0 && len != 0) {
        code_arr.pop_back();
    }
}

void huffman_tree::count_dp(int16_t v, int16_t u = -1) {
    if (v == -1 || v < ALPHABET_SIZE) {
        return;
    }
    dp[v].resize(256, -1);
    str_dp[v].resize(256, "");
    if (v == root) {
        for (int i = 0; i < 256; i++) {
            actual_vertex = v;
            for (int8_t j = 7; j >= 0; j--) {
                bool tmp = (i & (1 << j)) > 0;
                if (!tmp) {
                    actual_vertex = tree[actual_vertex].l;
                }
                else {
                    actual_vertex = tree[actual_vertex].r;
                }
                if (is_code()) {
                    str_dp[v][i] += get_if_code();
                }
            }
            dp[v][i] = actual_vertex;
        }
    }
    else {
        int16_t l, r;
        if (tree[u].l == v) {
            l = 0, r = 128;
        }
        else {
            l = 128, r = 256;
        }
        for (int16_t i = l; i < r; i++) {
            actual_vertex = dp[u][i];
            int16_t j = (i << 1) % 256;
            go_to(0);
            str_dp[v][j] = str_dp[u][i];
            if (is_code()) {
                str_dp[v][j] += get_if_code();
            }
            dp[v][j] = actual_vertex;
            actual_vertex = dp[u][i];
            go_to(1);
            str_dp[v][j + 1] = str_dp[u][i];
            if (is_code()) {
                str_dp[v][j + 1] += get_if_code();
            }
            dp[v][j + 1] = actual_vertex;
        }
    }
    count_dp(tree[v].l, v);
    count_dp(tree[v].r, v);
}