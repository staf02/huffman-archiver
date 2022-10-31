#include <cstring>
#include "buffered_writer.h"
#include "IO_exception.h"

buffered_writer::buffered_writer(std::ostream& dst) : buffer(new unsigned char[BUFF_LEN]), dst(dst) {}

buffered_writer::~buffered_writer() {
  if (pos != 0) {
      try {
          dst.write(reinterpret_cast<char *>(buffer), pos);
          pos = 0;
      }
      catch (const std::ios::failure &e) {
          throw IO_exception("error while writing file");
      } catch (const std::system_error &e) {
          throw IO_exception("error while writing file");
      }
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
        try {
            dst.write(reinterpret_cast<char *>(buffer), BUFF_LEN);
            pos = 0;
        }
        catch (const std::ios::failure &e) {
            throw IO_exception("error while writing file");
        } catch (const std::system_error &e) {
            throw IO_exception("error while writing file");
        }
    }
}
