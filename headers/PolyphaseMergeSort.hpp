#include "Tape.hpp"
#include <iostream>
#include <vector>
#include <memory>
#pragma once
enum io {
    READONLY = std::ios::in | std::ios::binary,
    WRITEONLY = std::ios::out | std::ios::binary | std::ios::app,
};

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