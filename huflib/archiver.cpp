#include "archiver.h"
#include "encoder.h"
#include "decoder.h"

void encode(std::istream& src, std::ostream& dst) {
    encoder e(src);
    e.save_to_file(dst);
}

void decode(std::istream& src, std::ostream& dst) {
    decoder d(src);
    d.save_to_file(dst);
}

