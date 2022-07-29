#include "huffman_tree.h"

node::node() : left(nullptr), right(nullptr) {
}

node::node(node* left, node* right) : left(left), right(right) {}

terminate_node::terminate_node(unsigned char const& c) : c(c) {};