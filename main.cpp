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

bool files_equal() {
    buffered_reader x("input.txt"), y("output.txt");
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
    encoder e("input.txt");
    e.encode();
    e.save_to_file("sample.jzip");
    decoder dec("sample.jzip");
    double t = clock();
    std::cout << t / 1000.0 << std::endl;
    dec.save_to_file("output.txt");
    double t1 = clock();
    std::cout << (t1 - t) / 1000.0 << std::endl;
}

int main() {
    archive();
    files_equal();
    return 0;
}