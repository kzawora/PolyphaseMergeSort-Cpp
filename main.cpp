#include "Block.hpp"
#include "PolyphaseMergeSort.cpp"
#include "Record.hpp"
#include "Tape.hpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#pragma once

#define DIST_UPPER_LIMIT 1e-3
#define DIST_LOWER_LIMIT 1e-4
static void GenerateTape(std::string path, int size) {
    std::fstream newFile;
    newFile.open(path, std::ios::out | std::ios::binary);
    std::default_random_engine gen(clock());
    std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT,
                                                DIST_UPPER_LIMIT);
    std::uniform_int_distribution<int> distInt(1, 15);
    auto generator = [&]() { return dist(gen); };
    auto generatorInt = [&]() { return distInt(gen); };
    for (int i = 0; i < size; i++) {
        int setSize = generatorInt();
        while (setSize--) {
            double num = generator();
            newFile.write(reinterpret_cast<char *>(&num), sizeof(double));
        }
        double separator = SEPARATOR_VALUE;
        newFile.write(reinterpret_cast<char *>(&separator), sizeof(double));
    }
    newFile.close();
}

int main() {
    GenerateTape("test.bin", (1 << 20));
    Tape tape1("test.bin", std::ios::binary | std::ios::in);
    std::remove("out.bin");
    Tape tape2("out.bin", std::ios::binary | std::ios::out | std::ios::app);
    int counter = 0;
    while (tape1.HasNext()) {
        auto x = tape1.GetNext();
        tape2.Push(x);
        counter++;
    }
    tape2.BlockWrite();

    return 0;
}
