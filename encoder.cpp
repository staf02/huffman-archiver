#include "encoder.h"
#include <queue>
#include <utility>
#include <memory>
#include <iostream>
#include "buffered_writer.h"

encoder::encoder() : source(), tree_root(nullptr), freq() {}

encoder::encoder(const char* filename) : source(filename), tree_root(nullptr), freq() {}

void encoder::open(const char* filename) {
    source.open(filename);
}

void encoder::encode() {
    count_freq();
    std::cout << "Frequency counted\n";
    build_tree();
}

void encoder::save_to_file(const char* filename) {
    buffered_writer out(filename);
    std::vector<bool> code;
    std::vector<std::vector<bool>> codes(256);
    gen_code(tree_root, codes, code);
    out.write(count_mod(codes));
    out.write(count_not_nulls());
    print_codes(out, codes);
    print_text(out, codes);
    out.close();
}

void encoder::count_freq() {
    unsigned char c;
    while (source.get_next(c)) {
        ++freq[c];
    }
}

bool operator<(std::pair<int, node*> a, std::pair<int, node*> b) {
    return a.first > b.first;
}

void encoder::build_tree() {
    std::priority_queue<std::pair<int, node*>> q;
    for (size_t i = 0; i < 256; ++i) {
        if (freq[i] != 0) {
            q.emplace(freq[i], new terminate_node(i));
        }
    }
    while (q.size() != 1) {
        auto v = q.top();
        q.pop();
        auto u = q.top();
        q.pop();
        q.emplace(v.first + u.first, new node(v.second, u.second));
    }
    tree_root = q.top().second;
}

void encoder::gen_code(node* t, std::vector<std::vector<bool>>& codes, std::vector<bool> &code) {
    if (t == nullptr) {
        return;
    }
    if (t->left == nullptr && t->right == nullptr) {
        if (code.empty()) {
            code.push_back(0);
        }
        codes[static_cast<terminate_node*>(t)->c] = code;
        return;
    }
    code.push_back(0);
    gen_code(t->left, codes, code);
    code.pop_back();
    code.push_back(1);
    gen_code(t->right, codes, code);
    code.pop_back();
}

void encoder::print_codes(buffered_writer& out, std::vector<std::vector<bool>>& codes) {
    for (size_t i = 0; i < 256; ++i) {
        if (codes[i].size() == 0) {
            continue;
        }
        unsigned char c = i, d = codes[i].size();
        out.write(c);
        out.write(d);
    }
    for (size_t i = 0; i < 256; ++i) {
        if (codes[i].size() == 0)
            continue;
        out.write_bit_array(codes[i]);
    }
}

void encoder::print_text(buffered_writer& out, std::vector<std::vector<bool>>& codes) {
    source.reset();
    unsigned char c;
    while (source.get_next(c)) {
        out.write_bit_array(codes[c]);
    }
}

unsigned char encoder::count_not_nulls() {
    unsigned char count = 0;
    for (size_t i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            ++count;
        }
    }
    return count;
}

unsigned char encoder::count_mod(std::vector<std::vector<bool>>& codes) {
    uint64_t len = 0;
    for (size_t i = 0; i < 256; ++i) {
        len += (freq[i] + 1) * codes[i].size();
    }
    return (8 - (len % 8)) % 8;
}
