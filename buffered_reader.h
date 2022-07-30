#pragma once
#include <cstdint>
#include <fstream>

class buffered_reader {
public:
    buffered_reader();
    buffered_reader(const char* filename);
    buffered_reader(buffered_reader &other) = delete;
    buffered_reader& operator=(buffered_reader& r) = delete;
    ~buffered_reader();
    
    void open(const char* filename);
    void close();
    void reset();

    bool has_next();
    bool get_next(unsigned char& c);

    unsigned char* read_char_array(size_t len);
    int16_t read_int16t();
    uint16_t read_uint16t();
    int32_t read_int32t();
    uint32_t read_uint32t();
    int64_t read_int64t();
    uint64_t read_uint64t();

private:
    size_t const BUFF_LEN = 1024;
    size_t pos, end_pos;
    unsigned char* buffer;
    std::ifstream input;

    void check_buffer();
    int64_t read_int(size_t);
};