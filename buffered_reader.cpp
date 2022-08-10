#include "buffered_reader.h"
#include <algorithm>
#include <bitset>
#include <iostream>

buffered_reader::buffered_reader(const char* filename) : buffer(new unsigned char[BUFF_LEN]) {
    src.open(filename, std::ios::binary);
    if (!src.is_open()) {
        throw std::runtime_error("cannot open file " + std::string(filename));
    }
}

buffered_reader::~buffered_reader() {
    delete[] buffer;
    src.close();
}

void buffered_reader::reset() {
    src.clear();
    src.seekg(0, std::ios::beg);
    check_buffer();
}

void buffered_reader::check_buffer() {
    if (pos != end_pos) {
        return;
    }
    src.read((char*) buffer, BUFF_LEN);
    end_pos = std::min(BUFF_LEN, static_cast<size_t>(src.gcount()));
    pos = 0;
}

bool buffered_reader::has_next() {
    check_buffer();
    return end_pos != 0;
}

bool buffered_reader::get_next(unsigned char& c){
    if (has_next()) {
        c = buffer[pos++];
        return true;
    }
    return false;
}