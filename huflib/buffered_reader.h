#pragma once
#include <cstdint>
#include <fstream>

class buffered_reader {
public:
    buffered_reader() = delete;
    buffered_reader(std::istream&);
    buffered_reader(buffered_reader &other) = delete;
    buffered_reader& operator=(buffered_reader& r) = delete;
    ~buffered_reader();
    
    void reset();

    bool has_next();
    bool get_next(unsigned char& c);

private:
    size_t const BUFF_LEN = 1 << 10;
    size_t pos = 0, end_pos = 0;
    unsigned char* buffer;
    std::istream& src;

    void check_buffer();
};