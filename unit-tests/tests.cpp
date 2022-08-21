#include "archiver.h"
#include "buffered_reader.h"
#include "buffered_writer.h"
#include "gtest/gtest.h"
#include <fstream>
#include <iostream>

bool check(const char* filename1, const char* filename2) {
  std::ifstream in1(filename1), in2(filename2);
  buffered_reader x(in1), y(in2);
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
  in1.close();
  in2.close();
  return true;
}

void compress(std::string a, std::string b) {
  std::ifstream src(a.c_str());
  std::ofstream dst(b.c_str());
  encode(src, dst);
  src.close();
  dst.close();
}

void decompress(std::string a, std::string b) {
  std::ifstream src(a.c_str());
  std::ofstream dst(b.c_str());
  try {
    decode(src, dst);
    src.close();
    dst.close();
  }
  catch (std::runtime_error &e) {
    src.close();
    dst.close();
    throw std::runtime_error("");
  }
}

void compress_decompress(std::string const& filename) {
  std::string arch_name = filename + ".huf";
  std::string new_filename = filename + ".dehuf";
  compress(filename, arch_name);
  decompress(arch_name, new_filename);
  bool t = check(filename.c_str(), new_filename.c_str());
  EXPECT_EQ(t, true);
}

void gen_random_file() {
  std::ofstream out("mytests/random.txt");
  buffered_writer dst(out);
  int sz = (rand()) % (1 << 14);
  for (int i = 0; i < sz; i++) {
    unsigned char c = rand() % 256;
    dst.write(c);
  }
  out.close();
  return;
}

void check_no_throw(std::string const &name) {
  EXPECT_NO_THROW(compress_decompress(name));
}

///
TEST(correctness, empty_file) {
  check_no_throw("mytests/empty.txt");
}

TEST(correctness, abracadabra) {
  check_no_throw("mytests/abracadabra.txt");
}

TEST(correctness, one_symbol) {
  std::ofstream out("mytests/aaaaaaaa.txt");
  buffered_writer dst(out);
  int sz = 1 << 25;
  for (int i = 0; i < sz; i++) {
    dst.write('a');
  }
  out.close();
  check_no_throw("mytests/aaaaaaaa.txt");
}

TEST(correctness, alphabet) {
  std::ofstream out("mytests/alphabet.txt");
  buffered_writer dst(out);
  int sz = 256;
  for (int i = 0; i < sz; i++) {
    dst.write(i);
  }
  out.close();
  check_no_throw("mytests/alphabet.txt");
}

TEST(correctness, txt) {
  std::string filename = "mytests/txt/";
  for (int i = 1; i <= 3; i++) {
    check_no_throw(filename + std::to_string(i) + ".txt");
  }
}

TEST(correctness, jpg) {
  std::string filename = "mytests/jpg/";
  for (int i = 1; i <= 5; i++) {
    check_no_throw(filename + std::to_string(i) + ".jpg");
  }
}

TEST(correctness, exe) {
  std::string filename = "mytests/exe/";
  for (int i = 1; i <= 5; i++) {
    check_no_throw(filename + std::to_string(i) + ".exe");
  }
}

TEST(correctness, pdf) {
  std::string filename = "mytests/pdf/1.pdf";
  compress_decompress(filename);
}

TEST(correctness, random_file) {
  gen_random_file();
  compress_decompress("mytests/random.txt");
}
