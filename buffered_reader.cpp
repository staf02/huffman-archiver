#include "buffered_reader.h"
#include <algorithm>
#include <bitset>
#include <iostream>

buffered_reader::buffered_reader(const char* filename) : buffer(new unsigned char[BUFF_LEN]) {
    input.open(filename, std::ios::binary);
    if (!input.is_open()) {
        throw std::exception();
        std::cerr << "can't open file";
    }
}

buffered_reader::~buffered_reader() {
    delete[] buffer;
    input.close();
}

void buffered_reader::reset() {
    input.clear();
    input.seekg(0, std::ios::beg);
    check_buffer();
}

void buffered_reader::check_buffer() {
    if (pos != end_pos) {
        return;
    }
    input.read((char*) buffer, BUFF_LEN);
    end_pos = std::min(BUFF_LEN, static_cast<size_t>(input.gcount()));
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