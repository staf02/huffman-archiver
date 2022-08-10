#include "buffered_writer.h"

buffered_writer::buffered_writer() : buffer(new unsigned char[BUFF_LEN]), dst() {};

buffered_writer::buffered_writer(const char* filename) : buffer(new unsigned char[BUFF_LEN]), dst(filename, std::ios::binary) {
    if (!dst.is_open()) {
        throw std::runtime_error("cannot open file " + std::string(filename));
    }
};

buffered_writer::~buffered_writer() {
    close();
    delete[] buffer;
}

void buffered_writer::close() {
    if (pos != 0) {
        dst.write((char*)buffer, pos);
    }
    dst.close();
}

void buffered_writer::write(unsigned char const& c) {
    write_buffer();
    buffer[pos++] = c;
}

void buffered_writer::write(std::string const& str) {
    for (unsigned char const& c : str) {
        write(c);
    }
}

void buffered_writer::write_buffer() {
    if (pos == BUFF_LEN) {
        dst.write((char*) buffer, BUFF_LEN);
        pos = 0;
    }
}
