#pragma once
#include <cstdint>
#include <fstream>

class buffered_reader {
public:
    buffered_reader();
    buffered_reader(const char*);
    buffered_reader(buffered_reader &other) = delete;
    buffered_reader& operator=(buffered_reader& r) = delete;
    ~buffered_reader();
    
    void open(const char* filename);
    void close();
    void reset();

    bool has_next();
    bool get_next(unsigned char& c);
    bool read_bit(bool&);
    unsigned char* read_char_array(size_t len);
    int16_t read_int16t();
    uint16_t read_uint16t();
    int32_t read_int32t();
    uint32_t read_uint32t();
    int64_t read_int64t();
    uint64_t read_uint64t();

    uint8_t bits_left();

private:
    size_t const BUFF_LEN = 1 << 16;
    size_t pos = 0, end_pos = 0, bit_pos = 8;
    unsigned char* buffer;
    unsigned char last = 0;
    std::ifstream input;
    bool file_opened = false;

    void check_buffer();
    int64_t read_int(size_t);
};