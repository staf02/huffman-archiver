#include "archiver.h"
#include "encoder.h"
#include "decoder.h"

void encode(std::string const& src, std::string const& dst) {
    encode(src.c_str(), dst.c_str());
}

void encode(const char* src, const char* dst) {
    encoder e(src);
    e.encode();
    e.save_to_file(dst);
}

void decode(std::string const& src, std::string const& dst) {
    decode(src.c_str(), dst.c_str());
}

void decode(const char* src, const char* dst) {
    decoder e(src);
    e.save_to_file(dst);
}
