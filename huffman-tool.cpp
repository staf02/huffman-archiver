//
// Created by staf02 on 08/08/22.
//

#include "huflib/archiver.h"
#include "IO_exception.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <iostream>
#include <iostream>

void help() {
    std::cout << "Compresses or decompresses files using the Huffman algorithm\n\n";
    std::cout << "--compress\t\tcompress file\n";
    std::cout << "--decompress\t\tdecompress file\n";
    std::cout << "--input [FILE]\t\tspecify input file\n";
    std::cout << "--output [FILE]\t\tspecify output file\n";
    std::cout << "--help\t\tdisplay this help and exit\n";
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cerr << "Expected cmd arguments\n";
        std::cerr << "Try 'huffman-tool --help' for more information.\n";
        return 1;
    }
    if (strcmp(argv[1], "--compress") == 0 || strcmp(argv[1], "--decompress") == 0) {
        if (argc != 6) {
            std::cerr << "Invalid count of arguments" << std::endl;
            std::cerr << "Try 'huffman-tool --help' for more information.\n";
            return 1;
        }
        char* input_file;
        char* output_file;
        bool found_input = false, found_output = false;
        for (size_t i = 2; i < argc; i++) {
            if (strcmp(argv[i], "--input") == 0) {
                if (found_input) {
                    std::cerr << "Input file has already been specified\n";
                    std::cerr << "Try 'huffman-tool --help' for more information.\n";
                    return 1;
                }
                if (i + 1 < argc) {
                    input_file = argv[i + 1];
                    i++;
                    found_input = true;
                }
                else {
                    std::cerr << "Input file not specified\n";
                    std::cerr << "Try 'huffman-tool --help' for more information.\n";
                    return 1;
                }
            }
            else if (strcmp(argv[i], "--output") == 0) {
                if (found_output) {
                    std::cerr << "Output file has already been specified\n";
                    std::cerr << "Try 'huffman-tool --help' for more information.\n";
                    help();
                    return 1;
                }
                if (i + 1 < argc) {
                    output_file = argv[i + 1];
                    i++;
                    found_output = true;
                }
                else {
                    std::cerr << "Output file not specified\n";
                    std::cerr << "Try 'huffman-tool --help' for more information.\n";
                    return 1;
                }
            }
            else {
                std::cerr << "Incorrect cmd arg\n";
                std::cerr << "Try 'huffman-tool --help' for more information.\n";
                return 1;
            }
        }
        try {
            std::ifstream src(input_file, std::ios::binary);
            std::ofstream dst(output_file, std::ios::binary);
            dst.exceptions(std::ofstream::failbit | std::ofstream::badbit);
            if (!src.is_open()) {
                throw IO_exception("cannot open input file " +
                    std::string(input_file));
            }
            if (!dst.is_open()) {
                throw IO_exception("cannot open output file " +
                    std::string(output_file));
            }
            switch (argv[1][2]) {
            case 'c':
                encode(src, dst);
                std::cout << input_file << " successfully compressed in " << output_file << "\n";
                std::cout << clock() / 1000.0 << "\n";
                break;
            case 'd':
                decode(src, dst);
                std::cout << input_file << " successfully decompressed in " << output_file << "\n";
                std::cout << clock() / 1000.0 << "\n";
                break;
            }
        }
        catch (const std::runtime_error& e) {
            std::cerr << e.what() << "\n";
            return 1;
        }
    }
    else if (strcmp(argv[1], "--help") == 0) {
        help();
    }
    else {
        std::cerr << "Incorrect cmd argument\n";
        std::cerr << "Try 'huffman-tool --help' for more information.\n";
    }
    return 0;
}