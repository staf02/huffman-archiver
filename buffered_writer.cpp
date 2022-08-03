#include "buffered_writer.h"

buffered_writer::buffered_writer() : buffer(new unsigned char[BUFF_LEN]), out() {};

buffered_writer::buffered_writer(const char* filename) : file_opened(true), buffer(new unsigned char[BUFF_LEN]), out(filename, std::ios::binary) {
    std::ios_base::sync_with_stdio(false);
    out.tie(0);
};

buffered_writer::~buffered_writer() {
    close();
    delete[] buffer;
}

void buffered_writer::open(const char* filename) {
    if (file_opened) {
        out.close();
    }
    out.open(filename);
    std::ios_base::sync_with_stdio(false);
    out.tie(0);
    file_opened = true;
}

void buffered_writer::close() {
    if (!file_opened) {
        return;
    }
    if (bit_pos != 0) {
        write(byte << (8 - bit_pos));
    }
    if (pos != 0) {
        out.write((char*)buffer, pos);
    }
    file_opened = false;
    out.close();
}

void buffered_writer::write(unsigned char const& c) {
    write_buffer();
    buffer[pos++] = c;
}

void buffered_writer::write(const char* str) {
    write(std::string(str));
}

void buffered_writer::write(std::string const& str) {
    for (unsigned char const& c : str) {
        write(c);
    }
}

void buffered_writer::write_uint16_t(uint16_t const& src) {
    char* arr = static_cast<char*>(operator new(16));
    memcpy(arr, &src, 2);
    write(arr);
}

void buffered_writer::write_bit(bool const& t) {
    if (bit_pos == 8) {
        write(byte);
        bit_pos = 0;
    }
    byte *= 2;
    if (t) {
        ++byte;
    }
    ++bit_pos;
}

void buffered_writer::write_buffer() {
    if (pos == BUFF_LEN) {
        out.write((char*) buffer, BUFF_LEN);
        pos = 0;
    }
}
