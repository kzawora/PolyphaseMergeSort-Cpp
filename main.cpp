#include "Block.hpp"
#include "PolyphaseMergeSort.cpp"
#include "Record.hpp"
#include "Tape.hpp"
#include <iostream>
#pragma once

int main() {
    Tape tape1("test.bin", std::ios::binary | std::ios::in);
    tape1.GenerateTape(1000);
    int counter = 0;
    while (tape1.HasNext()) {
        std::cout << ++counter << '\t' << tape1.GetNext();
    }
    return 0;
}
