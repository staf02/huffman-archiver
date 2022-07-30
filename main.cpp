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
    encoder e("12.jpg");
    e.encode();
    std::cout << "Successfuly encoded\n";
    e.save_to_file("sample.jzip");
    decoder dec("sample.jzip");
    dec.save_to_file("NEW_12.jpg");
    return 0;
}