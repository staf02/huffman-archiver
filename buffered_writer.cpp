#include "buffered_writer.h"

buffered_writer::buffered_writer() : buffer(new unsigned char[BUFF_LEN]), out() {};

buffered_writer::buffered_writer(const char* filename) : buffer(new unsigned char[BUFF_LEN]), out(filename, std::ios::binary) {};

buffered_writer::~buffered_writer() {
    close();
    delete[] buffer;
}

void buffered_writer::open(const char* filename) {
    out.open(filename);
}

void buffered_writer::close() {
    if (bit_pos != 0) {
        write(byte << (8 - bit_pos));
    }
    if (pos != 0) {
        out.write((char*)buffer, pos);
    }
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

void buffered_writer::write_bit_array(std::vector<bool> const& arr) {
    for (auto const& t : arr) {
        write_bit(t);
    }
}

void buffered_writer::write_buffer() {
    if (pos == BUFF_LEN) {
        out.write((char*) buffer, BUFF_LEN);
        pos = 0;
    }
}
