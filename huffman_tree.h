#pragma once
#include <memory>
#include <fstream>
#include <vector>

class node {
public:
    node* left, * right;

    node();
    node(node*, node*);
    ~node() = default;
};

class terminate_node : public node {
public:
    unsigned char c;
    terminate_node(unsigned char const& c);
};