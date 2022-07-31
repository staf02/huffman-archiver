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
    /*buffered_writer w("sample.jzip");
    w.write(1);
    w.write('a');
    w.write(8);
    w.write('x');
    w.write('x');*/
    /*
    encoder e("7.jpg");
    e.encode();
    std::cout << "Successfuly encoded\n";
    e.save_to_file("sample.jzip");*/
    decoder dec("sample.jzip");
    double t = clock();
    dec.save_to_file("output.txt");
    double t1 = clock();
    std::cout << t1 - t;
    return 0;
}