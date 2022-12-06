//
// Created by staf02 on 31/10/22.
//

#include <stdexcept>

#ifndef HUFFMAN_HUFFMAN_EXCEPTION_H
#define HUFFMAN_HUFFMAN_EXCEPTION_H

#endif //HUFFMAN_HUFFMAN_EXCEPTION_H

struct huffman_exception : public std::runtime_error {
    huffman_exception(const char* message);
};