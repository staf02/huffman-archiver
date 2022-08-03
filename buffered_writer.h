#pragma once
#include <vector>
#include <bitset>
#include <fstream>
#include <string>

class buffered_writer {
public:
    buffered_writer();
    buffered_writer(const char*);
    buffered_writer(buffered_writer& other) = delete;
    buffered_writer& operator=(buffered_writer& other) = delete;
    ~buffered_writer();

    void open(const char*);
    void close();

    void write(unsigned char const& c);
    void write(const char* str);
    void write(std::string const& str);
    void write_uint16_t(uint16_t const&);

    void write_bit(bool const&);

private:
    //pos is real length of buffer
    //when pos == 0, buffer is empty
    size_t const BUFF_LEN = 1 << 16;
    size_t pos = 0, bit_pos = 0;
    unsigned char byte = 0;
    unsigned char* buffer;
    std::ofstream out;
    bool file_opened = false;
    void write_buffer();
};