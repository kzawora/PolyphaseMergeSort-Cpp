#include "Block.hpp"
#include "CommonHeader.hpp"
#include "PMSTest.cpp"
#include "Record.hpp"
#include "Tape.hpp"
#include "helpers.cpp"
#include <iostream>
#pragma once

int main() {
    /*
    GenerateTape("test.bin", (1 << 5));
    /*

        std::ifstream  src("test.bin", std::ios::binary);
        std::ofstream  dst("test.bak",   std::ios::binary);
        dst << src.rdbuf();
        dst.flush();
        src.close();
        dst.close();
    /*
    PolyphaseMergeSort x("test.bin");


    const clock_t begin_time = clock();
    x.Sort();
    std::cout << float(clock() - begin_time) / CLOCKS_PER_SEC << std::endl;

    x.Print();
    Tape t("test.bin", std::ios::binary | std::ios::in);
    std::cout << t;
    while (t.HasNext()) {
        auto peek = t.PeekNext();
        std::cout << "current: " << t.GetCurrent() << "peek: " << peek;
        auto next = t.GetNext();
        std::cout << "next: " << next << std::endl;
        if (peek != next)
            std::cout << "NOT EQUAL!" << std::endl;
    }
    */
    PMSTest x("test.bin");
    x.Sort();

    x.Print();

    return 0;
}
