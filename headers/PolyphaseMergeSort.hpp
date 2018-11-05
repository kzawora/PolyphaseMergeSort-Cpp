#include "Tape.hpp"
#include <iostream>
#include <vector>
#include <memory>
#pragma once

class PolyphaseMergeSort {
    std::vector<std::shared_ptr<Tape>> tapes;
    std::string filename;
  public:
    PolyphaseMergeSort(std::shared_ptr<Tape> inputTape);
    PolyphaseMergeSort(std::string _filename);
    int Distribute();
    void Print();
    void Merge();
    void Sort();
    ~PolyphaseMergeSort();
};