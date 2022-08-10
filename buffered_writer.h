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

    void close();

    void write(unsigned char const& c);
    void write(std::string const& str);

private:
    //pos is real length of buffer
    //when pos == 0, buffer is empty
    size_t const BUFF_LEN = 1 << 10;
    size_t pos = 0;
    unsigned char* buffer;
    std::ofstream dst;

    void write_buffer();
};