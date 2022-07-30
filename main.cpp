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

int main() {
    encoder e("7.jpg");
    e.encode();
    std::cout << "Successfuly encoded\n";
    e.save_to_file("sample.jzip");
    decoder dec("sample.jzip");
    double t = clock();
    dec.save_to_file("out7.jpg");
    double t1 = clock();
    std::cout << t1 - t;
    return 0;
}