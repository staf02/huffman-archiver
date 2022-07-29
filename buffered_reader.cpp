#include "buffered_reader.h"
#include <algorithm>

buffered_reader::buffered_reader() : buffer(new unsigned char[BUFF_LEN]), pos(0), end_pos(0), input() {}

buffered_reader::buffered_reader(const char* filename) : buffer(new unsigned char[BUFF_LEN]), pos(0), end_pos(0) {
    input.open(filename, std::ios::binary);
    if (!input.is_open()) {
        throw "error while trying to open file";
    }
}

buffered_reader::~buffered_reader() {
    delete[] buffer;
    input.close();
}

void buffered_reader::open(const char* filename) {
    input.open(filename, std::ios::binary);
    if (!input.is_open()) {
        throw "error while trying to open file";
    }
}

void buffered_reader::close() {
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

int64_t buffered_reader::read_int(size_t bit_depth) {
    size_t len = std::min(bit_depth, static_cast<size_t>(input.gcount()));
    unsigned char* arr = read_char_array(len);
    int64_t res;
    memcpy(&res, arr, len);
    return res;
}

bool buffered_reader::has_next() {
    check_buffer();
    return end_pos != 0;
}

bool buffered_reader::get_next(unsigned char& c){
    if (has_next()) {
        c = get_next();
        return true;
    }
    return false;
}

bool buffered_reader::eof() {
    return pos == end_pos && input.eof();
}

unsigned char buffered_reader::get_next() {
    check_buffer();
    if (pos == end_pos) {
        throw std::runtime_error("end of file reached");
    }
    return buffer[pos++];
}

unsigned char* buffered_reader::read_char_array(size_t len) {
    check_buffer();
    if (static_cast<size_t>(input.gcount()) + end_pos - pos < len) {
        throw std::runtime_error("error: cannot read len chars");
    }
    unsigned char* res = static_cast<unsigned char*>(operator new(len * sizeof(unsigned char)));
    for (size_t i = 0; i < len; ++i) {
        get_next(res[i]);
    }
    return res;
}

int8_t buffered_reader::read_int8t() {
    return get_next();
}

uint8_t buffered_reader::read_uint8t() {
    return get_next();
}

int16_t buffered_reader::read_int16t() {
    return static_cast<int16_t>(read_int(2));
}

uint16_t buffered_reader::read_uint16t() {
    return static_cast<uint16_t>(read_int(2));
}

int32_t buffered_reader::read_int32t() {
    return static_cast<int32_t>(read_int(4));
}

uint32_t buffered_reader::read_uint32t() {
    return static_cast<uint32_t>(read_int(4));
}

int64_t buffered_reader::read_int64t() {
    return static_cast<int64_t>(read_int(8));
}

uint64_t buffered_reader::read_uint64t() {
    return static_cast<uint64_t>(read_int(8));
}