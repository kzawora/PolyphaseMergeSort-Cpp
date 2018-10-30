#include "Block.hpp"
#include "PolyphaseMergeSort.cpp"
#include "Record.hpp"
#include "Tape.hpp"
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#pragma once

#define DIST_LOWER_LIMIT 0
#define DIST_UPPER_LIMIT 100
static void GenerateTape(std::string path, int size) {
    std::fstream newFile;
    newFile.open(path, std::ios::out | std::ios::binary);
    std::default_random_engine gen(clock());
    std::uniform_real_distribution<double> dist(DIST_LOWER_LIMIT,
                                                DIST_UPPER_LIMIT);
    std::uniform_int_distribution<int> distInt(1, 1);
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
    GenerateTape("test.bin", (4*1024));
    PolyphaseMergeSort x("test.bin");
/*
    Tape t1("t2.bin", std::ios::in | std::ios::binary | std::ios::app);
    while (t1.HasNext())
        std::cout << t1.GetNext();
    //    std::cout << std::endl << "AFTER DIST" << std::endl;
*/
    x.Sort();
    x.Print();
    return 0;
}
