#include "Tape.hpp"
#include <vector>
#pragma once
class PolyphaseMergeSort {
    std::vector<std::shared_ptr<Tape>> tapes;

  public:
    PolyphaseMergeSort(std::shared_ptr<Tape> inputTape) {}
    PolyphaseMergeSort(std::string filename) {
        auto readMode = std::ios::in | std::ios::binary;
        auto writeMode = std::ios::out | std::ios::binary | std::ios::app;

        tapes.push_back(std::make_shared<Tape>(filename, readMode));
        std::remove("t2.bin");
        tapes.push_back(std::make_shared<Tape>("t2.bin", readMode));
        std::remove("t3.bin");
        tapes.push_back(std::make_shared<Tape>("t3.bin", readMode));
    }
    int Distribute() {
        std::shared_ptr<Tape> src, dest;
        while (src->HasNext()) {
            Record rec = src->GetNext();
            if (true) // TODO: ogarnac to
                dest = dest == tapes[1] ? tapes[2] : tapes[1];
        }
    }
    ~PolyphaseMergeSort() {}
};