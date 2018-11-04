#include "Block.hpp"
#include "PolyphaseMergeSort.hpp"
#include "Record.hpp"
#include "Tape.hpp"
#include "CommonHeader.hpp"
#include "helpers.cpp"
#include <iostream>
#pragma once

int main() {
    GenerateTape("test.bin", 400);
    PolyphaseMergeSort x("test.bin");
    std::ifstream  src("test.bin", std::ios::binary);
    std::ofstream  dst("test.bak",   std::ios::binary);
    dst << src.rdbuf();
    dst.flush();
    src.close();
    dst.close();
    const clock_t begin_time = clock();
    x.Sort();
    std::cout << float(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;

    x.Print();
    return 0;
}
