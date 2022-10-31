//
// Created by staf02 on 31/10/22.
//

#include <stdexcept>

#ifndef HUFFMAN_IO_EXCEPTION_H
#define HUFFMAN_IO_EXCEPTION_H

#endif //HUFFMAN_IO_EXCEPTION_H

struct IO_exception  : public std::runtime_error {
    explicit IO_exception(const char* message);
    explicit IO_exception(std::string const& message);
};