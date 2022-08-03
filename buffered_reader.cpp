#include "buffered_reader.h"
#include <algorithm>
#include <bitset>
#include <iostream>

buffered_reader::buffered_reader() : buffer(new unsigned char[BUFF_LEN]), input() {}

buffered_reader::buffered_reader(const char* filename) : buffer(new unsigned char[BUFF_LEN]), file_opened(true) {
    std::ios_base::sync_with_stdio(false);
    input.tie(0);
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

void buffered_reader::open(const char* filename) {
    if (file_opened) {
        input.close();
    }
    input.open(filename, std::ios::binary);
    if (!input.is_open()) {
        throw "error while trying to open file";
    }
    file_opened = true;
}

void buffered_reader::close() {
    if (!file_opened) {
        return;
    }
    file_opened = false;
    input.close();
}

void buffered_reader::reset() {
    input.clear();
    input.seekg(0, std::ios::beg);
    check_buffer();
}

void buffered_reader::check_buffer() {
    if (!file_opened) {
        throw("file not opened");
    }
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
        last = c;
        return true;
    }
    return false;
}

bool buffered_reader::read_bit(bool& bit) {
    unsigned char c;
    if (bit_pos == 8) {
        if (!get_next(c)) {
            return false;
        }
        bit_pos = 0;
    } 
    else {
        c = last; //pos == 0 <=> end_pos == 0 <=> !has_next()
    }
    bit = c & (1 << (7 - bit_pos));
    ++bit_pos;
    return true;

}

uint8_t buffered_reader::bits_left() {
    if (has_next()) {
        return 8;
    }
    return 8 - bit_pos;
}
