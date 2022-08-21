#pragma once
#include <fstream>
#include <string>
#include <vector>

class buffered_writer {
public:
    buffered_writer() = delete;
    buffered_writer(std::ostream&);
    buffered_writer(buffered_writer& other) = delete;
    buffered_writer& operator=(buffered_writer& other) = delete;
    ~buffered_writer();

    void write(unsigned char const& c);
    void write(std::string const& str);

private:
    //pos is real length of buffer
    //when pos == 0, buffer is empty
    size_t const BUFF_LEN = 1 << 10;
    size_t pos = 0;
    unsigned char* buffer;
    std::ostream& dst;

    void write_buffer();
};