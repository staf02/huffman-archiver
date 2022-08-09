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
    e.save_to_file("sample.jzip");
    decoder dec("sample.jzip");
    double t = clock();
    std::cout << t / 1000.0 << std::endl;
    dec.save_to_file("output.txt");
    double t1 = clock();
    std::cout << (t1 - t) / 1000.0 << std::endl;
}

void gen_file() {
    std::ofstream out("input.txt");
    int sz = (rand()) + 2;
    for (int i = 0; i < sz; i++) {
        unsigned char c = rand() % 256;
        out << c;
    }
    return;
}

int main() {
    /*srand(time(0));
    while (true) {
        gen_file();
        archive();
        std::cout << files_equal();
        if (!files_equal()) {
            return 0;
        }
    };*/
    archive();
    std::cout << files_equal();
    return 0;
}