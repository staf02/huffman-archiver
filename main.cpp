#include "buffered_reader.h"
#include "huffman_tree.h"
#include <fstream>
#include <memory>
#include <iostream>
#include <queue>
#include <array>
#include "encoder.h"
#include "decoder.h"
#include <bitset>
#include "buffered_writer.h"
#include <ctime>

bool files_equals(const char* a, const char* b) {
    buffered_reader x(a), y(b);
    unsigned char c, d;
    bool t1 = x.get_next(c), t2 = y.get_next(d);
    while (t1 || t2) {
        if (c != d) {
            return false;
        }
        if (!t1 || !t2) {
            return false;
        }
        t1 = x.get_next(c), t2 = y.get_next(d);
    }
    return true;
}

void archive() {
    encoder e("12.jpg");
    e.encode();
    std::cout << "Successfuly encoded\n";
    e.save_to_file("sample.jzip");
    decoder dec("sample.jzip");
    double t = clock();
    dec.save_to_file("out12.jpg");
    double t1 = clock();
    std::cout << t1 - t << std::endl;
}

int main() {
    archive();
    std::cout << files_equals("12.jpg", "out12.jpg");
    return 0;
}