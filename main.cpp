#include "buffered_reader.h"
#include "huffman_tree.h"
#include <fstream>
#include <memory>
#include <iostream>
#include <queue>
#include <array>
#include "encoder.h"
#include "decoder.h"

int main() {
    /*encoder e("calc1.exe");
    e.encode();
    std::cout << "Successfuly encoded\n";
    std::ofstream out("sample.jzip", std::ios::binary);
    e.save_to_file(out);*/
    decoder dec("sample.jzip");
    dec.save_to_file("NEW_calc.exe");
    return 0;
}