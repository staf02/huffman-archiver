#include "buffered_writer.h"

buffered_writer::buffered_writer(std::ostream& dst) : buffer(new unsigned char[BUFF_LEN]), dst(dst) {}

buffered_writer::~buffered_writer() {
  if (pos != 0) {
    dst.write((char*)buffer, pos);
  }
    delete[] buffer;
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